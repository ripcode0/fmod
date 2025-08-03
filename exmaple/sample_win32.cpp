#include <iostream>
#include <fmod/fmod.h>
#include <conio.h>

const char* FMOD_ErrorString(FMOD_RESULT result) {
    switch (result) {
        case FMOD_OK: return "No errors";
        case FMOD_ERR_BADCOMMAND: return "Bad command";
        case FMOD_ERR_CHANNEL_ALLOC: return "Channel alloc failed";
        case FMOD_ERR_CHANNEL_STOLEN: return "Channel stolen";
        case FMOD_ERR_DMA: return "DMA failure";
        case FMOD_ERR_DSP_CONNECTION: return "DSP connection error";
        case FMOD_ERR_DSP_DONTPROCESS: return "DSP don't process";
        case FMOD_ERR_DSP_FORMAT: return "DSP format error";
        case FMOD_ERR_DSP_INUSE: return "DSP already in use";
        case FMOD_ERR_DSP_NOTFOUND: return "DSP not found";
        case FMOD_ERR_FILE_BAD: return "File bad";
        case FMOD_ERR_FILE_COULDNOTSEEK: return "File couldn't seek";
        case FMOD_ERR_FILE_DISKEJECTED: return "File disk ejected";
        case FMOD_ERR_FILE_EOF: return "End of file";
        case FMOD_ERR_FILE_NOTFOUND: return "File not found";
        case FMOD_ERR_FORMAT: return "Unsupported file format";
        case FMOD_ERR_OUTPUT_INIT: return "Output device not initialized";
        default: return "Unknown FMOD error";
    }
}

#define FMOD_CHECK(res) \
    if ((res) != FMOD_OK) { \
        printf("FMOD error at %s:%d - %s\n", __FILE__, __LINE__, FMOD_ErrorString(res)); \
        exit(1); \
    }

int main(int args, char* argv[])
{
    FMOD_SYSTEM* system{};
    FMOD_SOUND* sound{};
    FMOD_SOUND* sound_sword{};
    FMOD_SOUND* sound_slash{};
    FMOD_CHANNEL* channel[3];

    FMOD_System_Create(&system);
    FMOD_System_Init(system, 512, FMOD_INIT_NORMAL, nullptr);

    FMOD_CHECK(FMOD_System_CreateSound(system, "bgm.mp3", FMOD_DEFAULT, nullptr,&sound));
    
    FMOD_CHECK(FMOD_System_CreateSound(system, "sword.mp3", FMOD_DEFAULT, nullptr,&sound_sword));

    FMOD_CHECK(FMOD_System_CreateSound(system, "slash.mp3", FMOD_DEFAULT, nullptr,&sound_slash));

    printf("Playing BGM ...\n");
    printf("Press D : Sword Attack\nPress S : Slash Attack\nPress Q : Quit");
    FMOD_System_PlaySound(system, sound, nullptr, false, &channel[0]);

    FMOD_BOOL is_running = true;
    
    while (is_running)
    {
        FMOD_System_Update(system);
        FMOD_Channel_IsPlaying(channel[0], &is_running);
        if(_kbhit()){
            int code = _getch();
            if(code == 'd' || code == 'D'){
                FMOD_System_PlaySound(system, sound_sword, nullptr, false, &channel[1]);
            }
            else if (code == 's' || code == 'S'){
                FMOD_System_PlaySound(system, sound_slash, nullptr, false, &channel[2]);
            }
            else if (code == 'q' || code == 'Q'){
                is_running = false;
            }
        }
    }
    
    FMOD_Sound_Release(sound);
    FMOD_Sound_Release(sound_sword);
    FMOD_Sound_Release(sound_slash);
    FMOD_System_Close(system);
    FMOD_System_Release(system);

    return 0;
}