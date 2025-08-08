#include <iostream>
#include <fmod/fmod.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <csignal>

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
        std::cerr << "FMOD error at " << __FILE__ << ":" << __LINE__ << " - " << FMOD_ErrorString(res) << std::endl; \
        exit(1); \
    }

int kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt); // 현재 터미널 속성 저장
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); // 비정규 입력, 에코 끄기
    tcsetattr(STDIN_FILENO, TCSANOW, &newt); // 새 속성 적용
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0); // 현재 파일 상태 플래그
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK); // non-block 설정

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // 원래 속성 복원
    fcntl(STDIN_FILENO, F_SETFL, oldf); // 플래그 복원

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

char getch() {
    struct termios oldt, newt;
    char ch;
    tcgetattr(STDIN_FILENO, &oldt); // 현재 설정 저장
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); // 입력 버퍼링 없이, 에코 없음
    tcsetattr(STDIN_FILENO, TCSANOW, &newt); // 설정 반영
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // 원래 설정 복원
    return ch;
}

int main(int argc, char* argv[])
{
    FMOD_SYSTEM* system{};
    FMOD_SOUND* sound{};
    FMOD_SOUND* sound_sword{};
    FMOD_SOUND* sound_slash{};
    FMOD_CHANNEL* channel[3] = {nullptr};

    FMOD_CHECK(FMOD_System_Create(&system));
    FMOD_CHECK(FMOD_System_Init(system, 512, FMOD_INIT_NORMAL, nullptr));

    FMOD_CHECK(FMOD_System_CreateSound(system, "bgm.mp3", FMOD_DEFAULT, nullptr, &sound));
    FMOD_CHECK(FMOD_System_CreateSound(system, "sword.mp3", FMOD_DEFAULT, nullptr, &sound_sword));
    FMOD_CHECK(FMOD_System_CreateSound(system, "slash.mp3", FMOD_DEFAULT, nullptr, &sound_slash));

    std::cout << "\n==============================\n";
    std::cout << "         Playing BGM ...       \n";
    std::cout << "==============================\n";
    std::cout << " Controls:\n";
    std::cout << "   [D] Sword Attack \n";
    std::cout << "   [S] Slash Attack \n";
    std::cout << "   [Q] Quit \n";
    std::cout << "==============================\n";
    
    FMOD_CHECK(FMOD_System_PlaySound(system, sound, nullptr, false, &channel[0]));

    FMOD_BOOL is_running = true;

    while (is_running)
    {
        FMOD_System_Update(system);
        FMOD_Channel_IsPlaying(channel[0], &is_running);

        if (kbhit()) {
            char code = getch();
            if (code == 'd' || code == 'D') {
                FMOD_System_PlaySound(system, sound_sword, nullptr, false, &channel[1]);
            }
            else if (code == 's' || code == 'S') {
                FMOD_System_PlaySound(system, sound_slash, nullptr, false, &channel[2]);
            }
            else if (code == 'q' || code == 'Q') {
                break;
            }
        }

        usleep(10000); 
    }

    FMOD_Sound_Release(sound);
    FMOD_Sound_Release(sound_sword);
    FMOD_Sound_Release(sound_slash);
    FMOD_System_Close(system);
    FMOD_System_Release(system);

    return 0;
}
