/*
 * MidiFile.h
 * Main MIDI file handling logic.
 */

#include <Arduino.h>
#include "LittleFS.h"
#include <stdio.h>
#include "pico/stdlib.h"

class MidiFile {
    public:
        enum TMidiEvent : uint8_t {
            NoteOff = 0x80,
            NoteOn = 0x90,
            Aftertouch = 0xA0,
            ControlChange = 0xB0,
            ProgramChange = 0xC0,
            ChannelPressure = 0xD0,
            PitchBend = 0xE0,
            SysEx = 0xF0,
            TempoChange = 0x51,
            // ^ this is technically a meta event, but
            // in order to let the controller know tempos, it needs to be sent as a midi event
        };
        enum TMetaEvent : uint8_t {
            Sequence = 0x00,
            Text = 0x01,
            Copyright = 0x02,
            TrackName = 0x03,
            InstrumentName = 0x04,
            Lyrics = 0x05,
            Marker = 0x06,
            CuePoint = 0x07,
            ChannelPrefix = 0x20,
            EndOfTrack = 0x2F,
            SetTempo = 0x51,
            SMPTEOffset = 0x54,
            TimeSignature = 0x58,
            KeySignature = 0x59,
            SequencerSpecific = 0x7F,
        };

        struct MidiEvent {
            TMidiEvent eventType;
            uint8_t channel = 0;
            uint8_t eventData[4];
            uint32_t deltaTick = 0;
            uint32_t absTick = 0;
        };
        struct MidiTrack {
            String trackName;
            String instName;
            std::vector<MidiEvent> vecEvents;
        };
        std::vector<MidiTrack> vecTracks;

	    uint16_t ticksPerBeat = 480;
        MidiFile() {}
        MidiFile(File file) {
            ParseFile(file);
        }

    bool ParseFile(File file) {
        Serial.printf("BEGIN PARSE");
        // helper funcs

        // swap the byte order of a 32bit int
        auto Swap32 = [](uint32_t n) {
            return (((n >> 24) & 0xff) | ((n << 8) & 0xff0000) | ((n >> 8) & 0xff00) | ((n << 24) & 0xff000000));
        };
        // swap the byte order of a 16bit int
        auto Swap16 = [](uint16_t n)
		{
			return ((n >> 8) | (n << 8));
		};
        // reads bytes from file and constructs a string of n length
        auto ReadString = [&file](uint32_t n)
		{
			String s;
			for (uint32_t i = 0; i < n; i++) s += file.read();
			return s;
		};

        /* 
         * read a compressed midi value, pretty much if the first bit of the first byte of a word is set to 1, it tells you
         * the next byte is needed to construct the full word, and for each successive byte that has the first bit set to 1,
         * the next byte is needed, etc. only the bottom 7 bits of each byte are used to construct the final value due to this 
         */
        auto ReadValue = [&file]()
		{
			uint32_t word = 0;
			uint8_t nextByte = 0;

			// read byte
			word = file.read();

			// check most significant bit, if set, more bytes need reading
			if (word & 0x80)
			{
				// get bottom 7 bits of read byte
				word &= 0x7F;
				do
				{
					// read next byte
					nextByte = file.read();

					// get word by setting bottom 7 bits, then shifting 7 bits
					word = (word << 7) | (nextByte & 0x7F);
				} 
				while (nextByte & 0x80); // loop while MSB is still set to continue reading bytes
			}

			// return final word (always 32bit uint)
			return word;
		};

        // BEGIN PARSING

        uint32_t buf32 = 0;
		uint16_t buf16 = 0;

		// read midi header (always fixed size)
		file.readBytes((char*)&buf32, sizeof(uint32_t));
		uint32_t fileID = Swap32(buf32);

        if (fileID != 'MThd') { // not a midi file, stop parsing.
            return false;
        }
		file.readBytes((char*)&buf32, sizeof(uint32_t));
		uint32_t headerLength = Swap32(buf32);
	    file.readBytes((char*)&buf16, sizeof(uint16_t));
		uint16_t format = Swap16(buf16);
		file.readBytes((char*)&buf16, sizeof(uint16_t));
		uint16_t trackChunks = Swap16(buf16);
	    file.readBytes((char*)&buf16, sizeof(uint16_t));
		ticksPerBeat = Swap16(buf16);

        // read tracks
        for (uint16_t n = 0; n < trackChunks; n++) {
            Serial.printf("== TRACK %d\n", n);
            // read track header
            file.readBytes((char*)&buf32, sizeof(uint32_t));
            uint32_t trackID = Swap32(buf32);
            if (trackID != 'MTrk') { // not a midi track, skip
                Serial.printf("[WARN] Unexpected track ID in track header! Skipping\n");
                continue;
            }
            file.readBytes((char*)&buf32, sizeof(uint32_t));
            uint32_t trackLength = Swap32(buf32);

            bool endOfTrack = false;
            uint8_t previousEvent = 0;

            vecTracks.push_back(MidiTrack());
            uint32_t timeAbs = 0;

            while (!endOfTrack) {
                uint32_t timeDelta = 0;
                
                uint8_t event = 0;

                // Read timeDelta
                timeDelta = ReadValue();
                timeAbs += timeDelta;
                
                // read midi event
                event = file.read();

                // support for midi running status
                // sends a message without its status/event byte and use the previous event byte, for optimization
                if (event < 0x80) {
                    event = previousEvent;
                    // seek 1 byte backward so midi data isn't read improperly
                    file.seek(-1, SeekMode::SeekCur);
                }

                switch ((event & 0xF0)) {
                    case TMidiEvent::NoteOff:{
                        previousEvent = event;
                        uint8_t channel = event & 0x0F;
                        uint8_t noteID = file.read();
                        uint8_t noteVel = file.read();
                        vecTracks[n].vecEvents.push_back({ TMidiEvent::NoteOff, channel, {noteID, noteVel}, timeDelta, timeAbs });
                        Serial.printf("MIDI_NOTE_OFF DEV: %d NOTE: %d VEL: %d T: %d\n", channel, noteID, noteVel, timeDelta);
                        break;}

                    case TMidiEvent::NoteOn:{
                        previousEvent = event;
                        uint8_t channel = event & 0x0F;
                        uint8_t noteID = file.read();
                        uint8_t noteVel = file.read();
                        // if velocity is 0, note on events are treated as note off events in the MIDI standard
                        vecTracks[n].vecEvents.push_back({ noteVel == 0 ? TMidiEvent::NoteOff : TMidiEvent::NoteOn, channel, {noteID, noteVel}, timeDelta, timeAbs });
                        Serial.printf("%s DEV: %d NOTE: %d VEL: %d T: %d\n", noteVel == 0 ? "MIDI_NOTE_OFF" : "MIDI_NOTE_ON", channel, noteID, noteVel, timeDelta);
                        break;}

                    case TMidiEvent::Aftertouch:{
                        previousEvent = event;
                        uint8_t channel = event & 0x0F;
                        uint8_t noteID = file.read();
                        uint8_t noteVel = file.read();
                        vecTracks[n].vecEvents.push_back({ TMidiEvent::Aftertouch, channel, {noteID, noteVel}, timeDelta, timeAbs });
                        Serial.printf("MIDI_AFTERTOUCH DEV: %d NOTE: %d\n", channel, noteID);
                        break;}

                    case TMidiEvent::ControlChange:{
                        previousEvent = event;
                        uint8_t channel = event & 0x0F;
                        uint8_t noteID = file.read();
                        uint8_t noteVel = file.read();
                        vecTracks[n].vecEvents.push_back({ TMidiEvent::ControlChange, channel, {noteID, noteVel}, timeDelta, timeAbs });
                        Serial.printf("MIDI_CC DEV: %d NOTE: %d\n", channel, noteID);
                        break;}

                    case TMidiEvent::ProgramChange:{
                        previousEvent = event;
                        uint8_t channel = event & 0x0F;
                        uint8_t programID = file.read();
                        Serial.printf("MIDI_PC DEV: %d PID: %d\n", channel, programID);
                        break;}

                    case TMidiEvent::ChannelPressure:{
                        previousEvent = event;
                        uint8_t channel = event & 0x0F;
                        uint8_t channelPressure = file.read();
                        Serial.printf("MIDI_CHPRESSURE DEV: %d PRESSURE: %d\n", channel, channelPressure);
                        break;}

                    case TMidiEvent::PitchBend:{
                        previousEvent = event;
                        uint8_t channel = event & 0x0F;
                        uint8_t LS7B = file.read();
                        uint8_t MS7B = file.read();
                        vecTracks[n].vecEvents.push_back({ TMidiEvent::PitchBend, channel, {LS7B, MS7B}, timeDelta, timeAbs });
                        Serial.printf("MIDI_PITCH_BEND LS7B: %d MS7B: %d\n", LS7B, MS7B);
                        break;}

                    case TMidiEvent::SysEx:{
                        if (event == 0xFF) {
                            // meta message
                            uint8_t metaType = file.read();
                            uint8_t msgLen = ReadValue();
                            // currently not much processing implemented for meta messages, but we need to read them
                            // otherwise it screws everything up
                            switch (metaType) {
                                case TMetaEvent::Sequence: {
                                    // read 2 bytes, sequence number.
                                    Serial.printf("META_SEQ %d\n", file.read() << file.read());
                                    break;
                                }
                                case TMetaEvent::Text: {
                                    Serial.printf("META_TXT %s\n", ReadString(msgLen));
                                    break;
                                }
                                case TMetaEvent::Copyright: {
                                    Serial.printf("META_CPY %s\n", ReadString(msgLen));
                                    break;
                                }
                                case TMetaEvent::TrackName: {
                                    vecTracks[n].trackName = ReadString(msgLen);
                                    Serial.printf("META_TRACK_NAME %s\n", vecTracks[n].trackName);
                                    break;
                                }
                                case TMetaEvent::InstrumentName: {
                                    vecTracks[n].instName = ReadString(msgLen);
                                    Serial.printf("META_INST_NAME %s\n", vecTracks[n].instName);
                                    break;
                                }
                                case TMetaEvent::Lyrics: {
                                    Serial.printf("META_LYRICS %s\n", ReadString(msgLen));
                                    break;    
                                }
                                case TMetaEvent::Marker: {
                                    Serial.printf("META_MARKER %s\n", ReadString(msgLen));
                                    break;
                                }
                                case TMetaEvent::CuePoint: {
                                    Serial.printf("META_CUE %s\n", ReadString(msgLen));
                                    break;
                                }
                                case TMetaEvent::ChannelPrefix: {
                                    Serial.printf("META_CHANNEL_PREFIX %d\n", file.read());
                                    break;
                                }
                                case TMetaEvent::EndOfTrack: {
                                    // set end of track variable to true so we can move on to the next track
                                    endOfTrack = true;
                                    Serial.printf("== END OF TRACK\n");
                                    break;
                                }
                                case TMetaEvent::SetTempo: {
                                    // read tempo and push event to events vector
                                    uint32_t tempo = 0;
                                    (tempo |= (file.read() << 16));
								    (tempo |= (file.read() << 8));
								    (tempo |= (file.read() << 0));
                                    // this is a little dumb, but don't worry about it
                                    // unpack tempo into 8 bit ints
                                    uint8_t tempo0 = tempo & 0xFF;
                                    uint8_t tempo1 = (tempo >> 8) & 0xFF;
                                    uint8_t tempo2 = (tempo >> 16) & 0xFF;
                                    uint8_t tempo3 = (tempo >> 24) & 0xFF;
                                    vecTracks[n].vecEvents.push_back({ TMidiEvent::TempoChange, 0, {tempo0, tempo1, tempo2, tempo3}, timeDelta, timeAbs });
                                    Serial.printf("MIDI_TEMPO_CHANGE TEMPO: %d\n", tempo);
                                    break;
                                }
                                case TMetaEvent::SMPTEOffset: {
                                    Serial.printf("META_SMPTE %d %d %d %d %d\n", file.read(), file.read(), file.read(), file.read(), file.read());
                                    break;
                                }
                                case TMetaEvent::TimeSignature: {
                                    Serial.printf("META_TIMESIG %d %d %d %d\n", file.read(), file.read(), file.read(), file.read());
                                    break;
                                }
                                case TMetaEvent::KeySignature: {
                                    Serial.printf("META_KEYSIG %d %d\n", file.read(), file.read());
                                    break;
                                }
                                case TMetaEvent::SequencerSpecific: {
                                    Serial.printf("META_SEQSPEC %s\n", ReadString(msgLen));
                                    break;
                                }
                                default:
                                    Serial.printf("[WARN] UNKNOWN META EVENT\n");
                                    break;
                            }
                        }
                        if (event == 0xF0) {
                            // SysEx Message Begin
                            Serial.printf("== SysEx Message Begin\n%s\n", ReadString(ReadValue()));
                        } if (event == 0xF7) {
                            // SysEx Message End
                            Serial.printf("== SysEx Message End\n%s\n", ReadString(ReadValue()));
                        }
                        break;}

                    default:{
                        // unrecognized byte, do nothing
                        Serial.printf("[WARN] UNRECOGNIZED MIDI EVENT\n");
                        break;}
                }
            }
        }

        Serial.printf("END PARSE");
        return true;
    }  
};

MidiFile::MidiTrack MergeTracks(std::vector<MidiFile::MidiTrack> tracksVec) {
    MidiFile::MidiTrack temp = MidiFile::MidiTrack();
    for (auto& track : tracksVec) {
        for (auto& event : track.vecEvents) {
            temp.vecEvents.push_back(event);
        }
    }
    return temp;
}

float TicksToSeconds(uint32_t deltaTick, uint32_t bpm, uint16_t tpb) {
    return (deltaTick / tpb / bpm) * 60;
}