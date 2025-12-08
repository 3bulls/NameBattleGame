// 【★★評価課題★★】 ネームバトラー(名前で戦闘プログラム)【★★評価課題★★】 

// 提出物：main.cpp  ★★ main.cpp を圧縮してはダメ ★★
//         暗号化.txt【コードをどのように数値化したのか指し示すコード表の資料、エクセルかパワポも可】
//         exeファイル        (2種類をまとめて圧縮すること)
//           セーブデータ.bin (2種類をまとめて圧縮すること)（■任意）
//           フローチャート（■任意）
//           画面遷移図（■任意）
// 課題目的：★★  独自ある面白いゲームであること  ★★
//           これまでの授業内容を理解していること
//           関数を使用していること
//           プロトタイプ宣言を使用していること
//           配列を使用していること
//           構造体を使用していること
//           ポインタを使用していること
//           ファイルの読み書きができること
//           効果的に画面描画できること
//           画面遷移の区切りが意識的についていること

// 【仕様】
// ・名前のASCII文字コードを利用してキャラクターを自動作成すること
//   (入力時に文字数の指定はしてよい。例.４～１２文字)
//   (文字サイズ数や、文字の組み合わせで上手く暗号化すること)
//   (ビット演算とか利用すると、少ない文字数からでも引き出しの量は増えるはず)
// ・登録しているキャラクターのデータを保存すること
// ・保存したキャラクターのデータを読み込み出来ること
// ・登録されているキャラクターデータを使用して、任意のキャラクター同士を選択して戦闘させること
// ・各画面遷移ごとにそれぞれ関数化していること
// ・戦闘処理のダメージ計算する処理を関数化させること
// ・ゲームループを使用してゲームが終了しないこと
// ・キャラクターの情報を構造体で管理していること
// ・キャラクターの情報をポインタで関数に受け渡ししていること
// ・タイトル画面から各画面へ遷移すること
// ・80ｘ25のサイズをフル活用すること
// ・使用できるインクルードファイルは以下のもの限定で作成する
// 	#include <iostream>	// 標準入出力(C++)
// 	#include <conio.h>	// コンソール入出力 「#include "conioex.h"」に置き換えてもよい
// 	#include <fstream>	// ファイル入出力(C++)


// 【要求】
// ・操作するインターフェース(UI)がわかりやすいようにする
//    (わかりにくいところは操作説明も画面に見えるようにする)
// ・プログラムの画面レイアウトの内容を凝った表示にする。
//   （画面全体を使って演出表現できている方が好ましい）
// ・★★ 他の人が遊びたくなるような面白ギミックも追加すること ★★

// 【要点】
// [感性－技術]：授業の範囲内でのテクニック
//         → 制限された機能を使用していないこと(協調性)
//         → 独自の技術であること・感心する技術であること
// [感性－美術]：画面の構成の美しさ
//         → 客観的な視点から感情を揺さぶらせること
//         → 視覚的にルールが理解しやすいこと・UIが良いこと
// [感性－企画]：ゲームのアイディア
//         → 興味を引く内容である・遊びたくなる内容である
//         → 課題として独創的な内容である
// [努力－熱意]：作りこみ内容
//         → コードのボリューム・関数やマクロ定義・定数の種類
//         → バグを出さないルートづくり
// [努力－誠実]：コードの見やすさ
//         → 変数・関数・構造体等の単語の命名センス
//         → インデント位置・コメント・ルールに従う能力
// [努力－設計]：アルゴリズムの資料作成
//         → 資料の見やすさ・手続き(処理)の整理
//         → アルゴリズムの理解力
// [理解－分析]：課題の理解力
//         → 指示に内容に記載されているルールを理解していること
//         → 仕様に対してコードへ実装する力があること
//         → プログラムの知識があること
// [理解－提出]：締め切り・フォーマットの管理
//         → コンパイルエラーが発生する
//         → フォーマット通りではない（圧縮の仕方、クラウドサービス等）
//         → ファイルが足りない・間違えている・壊れている 

#include "conioex.h"
#include <iostream>
#include <fstream>

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

enum GameState {
    STATE_TITLE,
    STATE_PLAYER_NAME_INPUT,
    STATE_PLAYER_CREATION,
    STATE_PLAYER_LOAD,
    STATE_ENEMY_CREATION,
    STATE_BATTLE,
    STATE_GAME_OVER,
    STATE_EXIT,
};

enum BattleTurn {
    PLAYER_TURN,
    ENEMY_TURN,
};

GameState GlobalgameState;

struct Character;

// my input setting for compatibility
void init_terminal();
void restore_terminal();
void set_terminal_mode(bool blocking);
int kbhit(void);
int my_getch(void);
void clearScreen();
void savePlayerToFile(Character* player);
bool loadPlayerFromFile(Character* player);

void getWidthAndHeight(char *art[], int *width, int *height) {
    *width = 0;
    *height = 0;
    for (int i = 0; art[i] != nullptr; i++) {
        int len = strlen(art[i]);
        if (len > *width) {
            *width = len;
        }
        (*height)++;
    }
}

enum JobType {
    WARRIOR,
    MAGE,
};

enum OptionType {
    YES,
    NO
};

char JobStrings[][10] = {
    "Warrior",
    "Mage",
};



enum PlayerNameInputState {
    INPUT_NEW_NAME,
    LOAD_EXISTING_NAME,
    EXIT_GAME,
};

struct Character {
    char name[13]; // 名前 (最大12文字 + 終端文字)
    JobType job;   // 職業
    int hp;        // ヒットポイント
    int maxHp;     // 最大ヒットポイント
    int mp;        // マジックポイント
    int attack;    // 攻撃力
    int magic;     // 魔力
    int physicDefense;   // 物理防御力
    int magicDefense;    // 魔法防御力
};


const char *banner[] = {
        "     _   _     _     __  __  _____       ",
        "    | \\ | |   / \\   |  \\/  || ____|      ",
        "    |  \\| |  / _ \\  | |\\/| ||  _|        ",
        "    | |\\  | / ___ \\ | |  | || |___       ",
        "    |_| \\_|/_/   \\_\\|_|  |_||_____|      ",
        "                                         ",
        " ____     _   _____  _____  _      _____ ",
        "| __ )   / \\ |_   _||_   _|| |    | ____|",
        "|  _ \\  / _ \\  | |    | |  | |    |  _|  ",
        "| |_) |/ ___ \\ | |    | |  | |___ | |___ ",
        "|____//_/   \\_\\|_|    |_|  |_____||_____|",
        nullptr
};

const char *battle_text[] = {
    " ____    _  _____ _____ _     _____   _   _   _ ",
    "| __ )  / \\|_   _|_   _| |   | ____| | | | | | |",
    "|  _ \\ / _ \\ | |   | | | |   |  _|   | | | | | |",
    "| |_) / ___ \\| |   | | | |___| |___  |_| |_| |_|",
    "|____/_/   \\_\\_|   |_| |_____|_____| (_) (_) (_)",
    nullptr
};

const char *menu_art[] = {
    " __  __                     ",
    "|  \\/  |  ___  _ __   _   _ ",
    "| |\\/| | / _ \\| '_ \\ | | | |",
    "| |  | ||  __/| | | || |_| |",
    "|_|  |_| \\___||_| |_| \\__,_",
    nullptr 
};

const char *creat_art[] = {
    "  ____                    _   ",
    " / ___| _ __  ___   __ _ | |_ ",
    "| |    | '__|/ _ \\ / _` || __|",
    "| |___ | |  |  __/| (_| || |_ ",
    " \\____||_|   \\___| \\__,_| \\__|",
    nullptr 
};

const char *enemy_art[] = {
    " _____                                   ",
    "| ____| _ __    ___  _ __ ___   _   _  ",
    "|  _|  | '_ \\  / _ \\| '_ ` _ \\ | | | | ",
    "| |___ | | | ||  __/| | | | | || |_| | ",
    "|_____||_| |_| \\___||_| |_| |_| \\__, | ",
    "                                |___/  ",
    nullptr
};

const char *load_art[] = {
    " _                       _ ",
    "| |     ___    __ _   __| |",
    "| |    / _ \\  / _` | / _` |",
    "| |___| (_) || (_| || (_| |",
    "|_____|\\___/  \\__,_| \\__,_|",
    nullptr
};

const char *warrior_art[] = {
    "         \\\\             /|",
    "         _!_           //",
    "        /___\\         //",
    "        [+++]        //",
    "     _ _\\^^^/_ _    //",
    "    ( )  '-'    ) \\//",
    "   / /\\   {&} | \\/ )\\",
    "  ( <_ \\     / \\  /",
    "   .-'\"\"`'-;:::<`\"",
    "  /         \\:::\\",
    "  |   {&}   ||\\  \\",
    "  \\         /) \\  )",
    "   '-.___.-'/   \\ \\",
    "      / /        \\ \\_",
    "    .|___\\       |___\\",
    NULL
};

const char *enemy_warrior_art[] = {
    "|\\             //",
    " \\\\           _!_",
    "  \\\\         /___\\",
    "   \\\\        [+++]",
    "    \\\\    _ _\\^^^/_ _",
    "     \\\\/ (    '-'  ( )",
    "     /( \\/ | {&}   /\\ \\",
    "       \\  / \\     / _> )",
    "        \"`   >:::;-'`\"\"'-.",
    "            /:::/         \\",
    "           /  /||   {&}   |",
    "          (  / (\\         /",
    "          / /   \\'-.___.-'",
    "        _/ /     \\ \\",
    "       /___|    /___|",
    NULL
};

const char *mage_art[] = {
    "                   .",
    "         /^\\     .",
    "    /\\   \"V\"",
    "   /__\\   I      O  o",
    "  //..\\\\  I     .",
    "  \\].`[/  I",
    "  /l\\/j\\  (]    .  O",
    " /. ~~ ,\\/I          .",
    " \\\\L__j^\\/I       o",
    "  \\/--v}  I     o   .",
    "  |    |  I   _________",
    "  |    |  I c(`       ')o",
    "  |    l  I   \\.     ,/",
    "_/j  L l\\_!  _//^---^\\\\_  c",
    NULL
};

const char *enemy_mage_art[] = {
    "                  /\\",
    "                |    |",
    "              --:'''':--",
    "                :'_' :",
    "                _:\"\":\\___",
    " ' '      ____.' :::     '._",
    ". *=====<<=)           \\    :",
    " .  '      '-'-'\\_      /'._.'",
    "                  \\====:_ \"\"",
    "                 .'     \\\\",
    "                :       :",
    "               /   :    \\",
    "              :   .      '.",
    "              :__:-:__.;--'",
    "              '-'   '-'",
    nullptr
};


Character playerCharacter;
Character enemyCharacter;

void creatPlayerData(const char* name, JobType job) {
    strncpy(playerCharacter.name, name, 12);
    playerCharacter.name[12] = '\0'; 
    playerCharacter.job = job;

    int nameNum = 0;
    for (int i = 0; name[i] != '\0'; i++) {
        nameNum += (int)name[i];
    }

    switch (job) {
        case WARRIOR:
            playerCharacter.hp = 200;
            playerCharacter.maxHp = 200;
            playerCharacter.mp = 30;
            playerCharacter.attack = nameNum % 20 + 10;
            playerCharacter.magic = nameNum % 10 + 5;
            playerCharacter.physicDefense = nameNum % 5 + 10;
            playerCharacter.magicDefense = nameNum % 5 + 5;
            break;
        case MAGE:
            playerCharacter.hp = 100;
            playerCharacter.maxHp = 100;
            playerCharacter.mp = 100;
            playerCharacter.attack = nameNum % 10 + 5;
            playerCharacter.magic = nameNum % 20 + 15;
            playerCharacter.physicDefense = nameNum % 5 + 5;
            playerCharacter.magicDefense = nameNum % 5 + 10;
            break;
    }
}

void createEnemyData(const char* name) {
    strncpy(enemyCharacter.name, name, 12);
    enemyCharacter.name[12] = '\0'; 
    enemyCharacter.job = (strlen(name) % 2 == 0) ? WARRIOR : MAGE;

    int nameNum = 0;
    for (int i = 0; name[i] != '\0'; i++) {
        nameNum += (int)name[i];
    }

    switch (enemyCharacter.job) {
        case WARRIOR:
            enemyCharacter.hp = 200;
            enemyCharacter.maxHp = 200;
            enemyCharacter.mp = 30;
            enemyCharacter.attack = nameNum % 20 + 10;
            enemyCharacter.magic = nameNum % 10 + 5;
            enemyCharacter.physicDefense = nameNum % 5 + 10;
            enemyCharacter.magicDefense = nameNum % 5 + 5;
            break;
        case MAGE:
            enemyCharacter.hp = 100;
            enemyCharacter.maxHp = 100;
            enemyCharacter.mp = 100;
            enemyCharacter.attack = nameNum % 10 + 5;
            enemyCharacter.magic = nameNum % 20 + 15;
            enemyCharacter.physicDefense = nameNum % 5 + 5;
            enemyCharacter.magicDefense = nameNum % 5 + 10;
            break;
    }
}

void drawArtAtXY(int x, int y, const char* art[]) {
    int artWidth, artHeight;
    getWidthAndHeight((char**)art, &artWidth, &artHeight);

    // Center the art if x or y is negative
    if (x < 0) {
        x = (SCREEN_WIDTH - artWidth) / 2 + 1;
    }

    if (y < 0) {
        y = (SCREEN_HEIGHT - artHeight) / 2 + 1;
    }

    for (int i = 0; i < artHeight; i++) {
        gotoxy(x, y + i);
        std::cout << art[i];
    }
    std::cout.flush();
}

void drawThePlatform() {
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            if (x==0&&y==0) {
                std::cout << "┏";
            } else if (x==SCREEN_WIDTH-1&&y==0) {
                std::cout << "┓";
            } else if (x==0&&y==SCREEN_HEIGHT-1) {
                std::cout << "┗";
            } else if (x==SCREEN_WIDTH-1&&y==SCREEN_HEIGHT-1) {
                std::cout <<  "┛";
            } else if (x==0 || x==SCREEN_WIDTH-1) {
                std::cout << "┃";
            } else if (y==0 || y==SCREEN_HEIGHT-1) {
                std::cout << "━";
            } else {
                std::cout << " ";
            }
        }
        std::cout << std::endl;
    }
}

void drawStartScreen() {
    clearScreen();
    drawThePlatform();
    drawArtAtXY(-1, 3, banner);
    const char* pressEnterKey[] =  {"      Press Enter Key to Start      ", nullptr};
    drawArtAtXY(-1, 17, pressEnterKey);
    const char* instructions[] = {" Use W/S to navigate menus, Enter to select ", nullptr};
    drawArtAtXY(-1, 19, instructions);
    gotoxy(1, SCREEN_HEIGHT); // Move cursor out of the way
}

 void handleStartScreenInput() {
    char ch = my_getch();
    while (ch != '\r' && ch != '\n') {
        ch = my_getch();
    }
    GlobalgameState = STATE_PLAYER_NAME_INPUT;
}

void drawPlayerNameInputScreen() {
    clearScreen();
    drawThePlatform();
    drawArtAtXY(-1, 3, menu_art);
    const char* createNewChar[] = {"Create New Character", nullptr};
    drawArtAtXY(25, 10, createNewChar);
    const char* loadExistingChar[] = {"Load Existing Character", nullptr};
    drawArtAtXY(25, 12, loadExistingChar);
    const char* exitGame[] = {"Exit Game", nullptr};
    drawArtAtXY(25, 14, exitGame);
    const char* instructions[] = {"    W/S to navigate, Enter to select    ", nullptr};
    drawArtAtXY(-1, 18, instructions);
    const char* arrow[] = {"->", nullptr};
    drawArtAtXY(20, 10, arrow);
    gotoxy(1, SCREEN_HEIGHT); // Move cursor out of the way
}

void clearArrowFromMenu(int selection) {
    gotoxy(20, 10 + selection * 2);
    std::cout << "  "; // Clear arrow
    gotoxy(1, SCREEN_HEIGHT); // Move cursor out of the way
}

void drawArrowAtMenu(int selection) {
    const char* arrow[] = {"->", nullptr};
    drawArtAtXY(20, 10 + selection * 2, arrow);
    gotoxy(1, SCREEN_HEIGHT); // Move cursor out of the way
}

void handlePlayerNameInput() {
    init_terminal();
    set_terminal_mode(false); 
    PlayerNameInputState inputState = INPUT_NEW_NAME;

    while (true) {
        if (kbhit()) {
            char ch = my_getch();
            clearArrowFromMenu(inputState);
            if (ch == 'w' || ch == 'W') {
                inputState = static_cast<PlayerNameInputState>((inputState - 1 + 3) % 3);
            } else if (ch == 's' || ch == 'S') {
                inputState = static_cast<PlayerNameInputState>((inputState + 1) % 3);
            } else if (ch == '\r' || ch == '\n') {
                switch (inputState) {
                    case INPUT_NEW_NAME:
                        GlobalgameState = STATE_PLAYER_CREATION;
                        return;
                    case LOAD_EXISTING_NAME:
                        GlobalgameState = STATE_PLAYER_LOAD;
                        return;
                    case EXIT_GAME:
                        GlobalgameState = STATE_EXIT;
                        return;
                }
            }
            drawArrowAtMenu(inputState);
        }
    }
    restore_terminal();
}


void drawCreateCharacterScreen(){
    clearScreen();
    drawThePlatform();
    drawArtAtXY(-1, 3, creat_art);
    const char* prompt[] = {"Enter your character name (max 12 chars): ", nullptr};
    drawArtAtXY(25, 10, prompt);
    gotoxy(1, SCREEN_HEIGHT); // Move cursor out of the way
}

void handleCreateCharacterInput(){
    char name[13] = {0};
    int index = 0;
    init_terminal();
    set_terminal_mode(true);

    gotoxy(30, 12);
    setcursortype(NORMALCURSOR);
    while (true) {
        char ch = my_getch();
        if (ch == '\r' || ch == '\n') {
            break;
        } else if (ch == '\b' || ch == 127) { 
            if (index > 0) {
                index--;
                name[index] = '\0';
                const char* space[] = {" ", nullptr};
                drawArtAtXY(30 + index, 12, space);
                gotoxy(30 + index, 12);
            }
        } else if (index < 12 && isprint(ch)) {
            name[index++] = ch;
            char charStr[] = {ch, '\0'};
            const char* charToDraw[] = {charStr, nullptr};
            drawArtAtXY(30 + index - 1, 12, charToDraw);
            gotoxy(30 + index, 12); 
        } else if (index == 12) {
            const char* nameTooLong[] = {"Name too long! Max 12 characters.", nullptr};
            drawArtAtXY(-1, 14, nameTooLong);
            gotoxy(30 + index, 12); 
        }
    }
    name[index] = '\0';

    clearScreen();
    drawThePlatform();
    drawArtAtXY(-1, 3, creat_art);
    setcursortype(NOCURSOR);
    char nameDisplay[30];
    snprintf(nameDisplay, sizeof(nameDisplay), "Name: %s", name);
    const char* nameDisplayArt[] = {nameDisplay, nullptr};
    drawArtAtXY(-1, 10, nameDisplayArt);
    
    const char* jobSelectionStrings[] = {
        "Select Your Job:",
        "1. Warrior",
        "2. Mage",
        nullptr
    };
    JobType selectedJob = WARRIOR;
    drawArtAtXY(-1, 12, jobSelectionStrings);
    int jobWidth, jobHeight;
    getWidthAndHeight((char**)jobSelectionStrings, &jobWidth, &jobHeight);
    const char* arrow[] = {"->", nullptr};
    drawArtAtXY((SCREEN_WIDTH - jobWidth)/2 - 2, 13, arrow);

    init_terminal();
    set_terminal_mode(false); 
    while (true) {
        if (kbhit()) {
            char ch = my_getch();
            if (ch == 'w' || ch == 'W') {
                selectedJob = static_cast<JobType>((selectedJob - 1 + 2) % 2);
            } else if (ch == 's' || ch == 'S') {
                selectedJob = static_cast<JobType>((selectedJob + 1) % 2);
            } else if (ch == '\r' || ch == '\n') {
                break;
            }
            for (int i = 0; i < 2; i++) {
                gotoxy((SCREEN_WIDTH - jobWidth)/2 - 2, 13 + i);
                if (i == selectedJob) {
                    drawArtAtXY((SCREEN_WIDTH - jobWidth)/2 - 2, 13 + i, arrow);
                } else {
                    const char* space[] = {"  ", nullptr};
                    drawArtAtXY((SCREEN_WIDTH - jobWidth)/2 - 2, 13 + i, space);
                }
            }
        }
    }
    restore_terminal();

    clearScreen();
    drawThePlatform();
    drawArtAtXY(-1, 3, creat_art);
    const char* creationComplete[] = {"Character Creation Complete!", nullptr};
    drawArtAtXY(-1, 10, creationComplete);
    char finalNameDisplay[30];
    snprintf(finalNameDisplay, sizeof(finalNameDisplay), "Name: %s", name);
    const char* finalNameArt[] = {finalNameDisplay, nullptr};
    drawArtAtXY(-1, 12, finalNameArt);
    char finalJobDisplay[30];
    snprintf(finalJobDisplay, sizeof(finalJobDisplay), "Job: %s", JobStrings[selectedJob]);
    const char* finalJobArt[] = {finalJobDisplay, nullptr};
    drawArtAtXY(-1, 14, finalJobArt);

    creatPlayerData(name, selectedJob);
    const char* storeOption[] = {
        "Do you want to save your character?", 
        "1. Yes",
        "2. No",
        nullptr};
    drawArtAtXY(-1, 16, storeOption);
    int storeWidth, storeHeight;
    getWidthAndHeight((char**)storeOption, &storeWidth, &storeHeight);
    drawArtAtXY((SCREEN_WIDTH - storeWidth)/2 - 2, 17, arrow);
    OptionType storeSelection = YES;

    init_terminal();
    set_terminal_mode(false); 
        while (true) {
        if (kbhit()) {
            char ch = my_getch();
            if (ch == 'w' || ch == 'W') {
                storeSelection = static_cast<OptionType>((storeSelection - 1 + 2) % 2);
            } else if (ch == 's' || ch == 'S') {
                storeSelection = static_cast<OptionType>((storeSelection + 1) % 2);
            } else if (ch == '\r' || ch == '\n') {
                break;
            }
            for (int i = 0; i < 2; i++) {
                gotoxy((SCREEN_WIDTH - storeWidth)/2 - 2, 17 + i);
                if (i == storeSelection) {
                    drawArtAtXY((SCREEN_WIDTH - storeWidth)/2 - 2, 17 + i, arrow);
                } else {
                    const char* space[] = {"  ", nullptr};
                    drawArtAtXY((SCREEN_WIDTH - storeWidth)/2 - 2, 17 + i, space);
                }
            }
        }
    }

    if (storeSelection == YES) {
        savePlayerToFile(&playerCharacter);
    }

    my_getch();
    restore_terminal();
    GlobalgameState = STATE_ENEMY_CREATION;
}

void drawEnemyCreationScreen()
{
    clearScreen();
    drawThePlatform();
    drawArtAtXY(-1, 3, enemy_art);
    const char* prompt[] = {"Enter Enemy character name (max 12 chars): ", nullptr};
    drawArtAtXY(-1, 10, prompt);
    gotoxy(1, SCREEN_HEIGHT); // Move cursor out of the way
}

void handleEnemyCreationInput()
{
    char name[13] = {0};
    int index = 0;
    init_terminal();
    set_terminal_mode(true);

    gotoxy(30, 12);
    setcursortype(NORMALCURSOR);
    while (true) {
        char ch = my_getch();
        if (ch == '\r' || ch == '\n') {
            break;
        } else if (ch == '\b' || ch == 127) { 
            if (index > 0) {
                index--;
                name[index] = '\0';
                const char* space[] = {" ", nullptr};
                drawArtAtXY(30 + index, 12, space);
                gotoxy(30 + index, 12);
            }
        } else if (index < 12 && isprint(ch)) {
            name[index++] = ch;
            char charStr[] = {ch, '\0'};
            const char* charToDraw[] = {charStr, nullptr};
            drawArtAtXY(30 + index - 1, 12, charToDraw);
            gotoxy(30 + index, 12); 
        } else if (index == 12) {
            const char* nameTooLong[] = {"Name too long! Max 12 characters.", nullptr};
            drawArtAtXY(-1, 14, nameTooLong);
            gotoxy(30 + index, 12); 
        }
    }
    name[index] = '\0';
    restore_terminal();

    createEnemyData(name);
    const char* creationComplete[] = {"Enemy Creation Complete!", nullptr};
    drawArtAtXY(-1, 16, creationComplete);

    const char* pressKey[] = {"Press any key to continue...", nullptr};
    drawArtAtXY(-1, 18, pressKey);
    my_getch();
    GlobalgameState = STATE_BATTLE;
}

void drawPlayerLoadScreen(){
    clearScreen();
    drawThePlatform();
    drawArtAtXY(-1, 3, load_art);
    const char* loading[] = {"Loading Player Data...", nullptr};
    drawArtAtXY(-1, 10, loading);
    gotoxy(1, SCREEN_HEIGHT); // Move cursor out of the way
}

void handlePlayerLoadInput(){
    init_terminal();
    set_terminal_mode(true);
    if (loadPlayerFromFile(&playerCharacter)) {
        const char* loadSuccess[] = {"Player Data Loaded Successfully!", nullptr};
        drawArtAtXY(-1, 12, loadSuccess);
        GlobalgameState = STATE_ENEMY_CREATION;
    } else {
        const char* loadFail[] = {"Failed to Load Player Data!", nullptr};
        drawArtAtXY(-1, 12, loadFail);
        GlobalgameState = STATE_PLAYER_NAME_INPUT;
    }
    const char* pressKey[] = {"Press any key to continue...", nullptr};
    drawArtAtXY(-1, 14, pressKey);
    my_getch();
    restore_terminal();
}

void drawCharacterInfoAtXY(int x, int y, Character* character)
{
    char infoLines[6][40];
    snprintf(infoLines[0], sizeof(infoLines[0]), "Name: %s", character->name);
    snprintf(infoLines[1], sizeof(infoLines[1]), "Job: %s", JobStrings[character->job]);
    snprintf(infoLines[2], sizeof(infoLines[2]), "HP: %d/%d", character->hp, character->maxHp);
    snprintf(infoLines[3], sizeof(infoLines[3]), "MP: %d", character->mp);
    snprintf(infoLines[4], sizeof(infoLines[4]), "ATK: %d  MAG: %d", character->attack, character->magic);
    snprintf(infoLines[5], sizeof(infoLines[5]), "P.DEF: %d  M.DEF: %d", character->physicDefense, character->magicDefense);

    for (int i = 0; i < 6; i++) {
        gotoxy(x, y + i);
        std::cout << infoLines[i];
    }
    std::cout.flush();
}

void drawBattleProcess(BattleTurn turn)
{
    while (playerCharacter.hp > 0 && enemyCharacter.hp > 0) {
        clearScreen();
        drawThePlatform();
        // Draw Player
        drawArtAtXY(3, 3, playerCharacter.job == WARRIOR ? warrior_art : mage_art);
        // Draw Enemy
        drawArtAtXY(50, 3, enemyCharacter.job == WARRIOR ? enemy_warrior_art : enemy_mage_art);   

        int playerDamage = playerCharacter.attack - enemyCharacter.physicDefense;
        int enemyDamage = enemyCharacter.attack - playerCharacter.physicDefense;
        int playerMagicDamage = playerCharacter.magic - enemyCharacter.magicDefense;
        int enemyMagicDamage = enemyCharacter.magic - playerCharacter.magicDefense;

        playerDamage *= (rand() % 51 + 90) / 100.0;
        enemyDamage *= (rand() % 51 + 90) / 100.0;
        playerMagicDamage *= (rand() % 51 + 90) / 100.0;
        enemyMagicDamage *= (rand() % 51 + 90) / 100.0;

        if (turn == PLAYER_TURN) {
            int trueDamage;
            if (playerCharacter.job == WARRIOR) {
                trueDamage = (playerDamage > 0) ? playerDamage : 1;
                enemyCharacter.hp -= trueDamage;
            } else {
                trueDamage = (playerMagicDamage > 0) ? playerMagicDamage : 1;
                enemyCharacter.hp -= trueDamage;
            }
            const char* attackType = (playerCharacter.job == WARRIOR) ? "Physical Damage!!!" : "Magic Damage!!!";
            char damageStr[30];
            snprintf(damageStr, sizeof(damageStr), "Causing %d damage", trueDamage);
            const char* playerAttackText[] = {
                "Your Turn!", 
                "You attack enemy by ",
                attackType,
                damageStr,
                nullptr};
            drawArtAtXY(-1, SCREEN_HEIGHT - 5, playerAttackText);
            // playAttackVideo(turn);
            turn = ENEMY_TURN;
        } else {
            if (enemyCharacter.job == WARRIOR) {
                playerCharacter.hp -= (enemyDamage > 0) ? enemyDamage : 1;
            } else {
                playerCharacter.hp -= (enemyMagicDamage > 0) ? enemyMagicDamage : 1;
            }
            const char* attackType = (enemyCharacter.job == WARRIOR) ? "Physical Damage!!!" : "Magic Damage!!!";
            
            char damageStr[30];
            snprintf(damageStr, sizeof(damageStr), "Causing %d damage", (enemyCharacter.job == WARRIOR) ? ((enemyDamage > 0) ? enemyDamage : 1) : ((enemyMagicDamage > 0) ? enemyMagicDamage : 1));
            const char* enemyAttackText[] = {
                "Enemy's Turn!", 
                "Enemy attacks you by ",
                attackType,
                damageStr,
                nullptr};
            drawArtAtXY(-1, SCREEN_HEIGHT - 5, enemyAttackText);
            // playAttackVideo(turn);
            turn = PLAYER_TURN;
        }

        // Draw HP
        int nOfPlayer = playerCharacter.hp * 20 / playerCharacter.maxHp;
        int nOfEnemy = enemyCharacter.hp * 20 / enemyCharacter.maxHp;
        char playerHpBar[25] = "HP: [";
        for (int i = 0; i < 20; i++) {
            if (i < nOfPlayer) {
                strcat(playerHpBar, "=");
            } else {
                strcat(playerHpBar, " ");
            }
        }
        strcat(playerHpBar, "]");
        char enemyHpBar[25] = "HP: [";
        for (int i = 0; i < 20; i++) {
            if (i < nOfEnemy) {
                strcat(enemyHpBar, "=");
            } else {
                strcat(enemyHpBar, " ");
            }
        }
        strcat(enemyHpBar, "]");
        const char* playerHpArt[] = {playerHpBar, nullptr};
        const char* enemyHpArt[] = {enemyHpBar, nullptr};
        drawArtAtXY(3, 2, playerHpArt);
        drawArtAtXY(50, 2, enemyHpArt);

        drawCharacterInfoAtXY(3, SCREEN_HEIGHT - 6, &playerCharacter);
        drawCharacterInfoAtXY(60, SCREEN_HEIGHT - 6, &enemyCharacter);
        while (char ch = my_getch()) {
            if (ch == '\r' || ch == '\n') {
                break;
            }
        }
    }
    GlobalgameState = STATE_GAME_OVER;
}

void drawBattleScreen(){
    clearScreen();
    drawThePlatform();
    drawArtAtXY(-1, 8, battle_text);
    const char* battleRule[] = {"Press enter key to proceed each turn.", nullptr};
    drawArtAtXY(-1, 16, battleRule);
    while (char ch = my_getch())
    {
        if (ch == '\r' || ch == '\n') {
            break;
        }
    }

    gotoxy(1, SCREEN_HEIGHT); // Move cursor out of the way
}

void handleBattleInput(){
    setcursortype(NOCURSOR);
    init_terminal();
    set_terminal_mode(false);
    BattleTurn turn = PLAYER_TURN;
    drawBattleProcess(turn);
    char ch = my_getch();
}

void drawGameOverScreen()
{
    clearScreen();
    drawThePlatform();
    const char* gameOverText[] = {
        "  ____                         ___                 ",
        " / ___| __ _ _ __ ___   ___   / _ \\__   _____ _ __ ",
        "| |  _ / _` | '_ ` _ \\ / _ \\ | | | \\ \\ / / _ \\ '__|",
        "| |_| | (_| | | | | | |  __/ | |_| |\\ V /  __/ |   ",
        " \\____|\\__,_|_| |_| |_|\\___|  \\___/  \\_/ \\___|_|   ",
        nullptr
    };
    drawArtAtXY(-1, 5, gameOverText);

    const char* resultText;
    if (playerCharacter.hp > 0) {
        resultText = "You Win!";
    } else {
        resultText = "You Lose!";
    }
    const char* resultArt[] = {resultText, nullptr};
    drawArtAtXY(-1, 15, resultArt);
}

void handleGameOverInput()
{
    const char* endOption[] = {
        "Do you want to play again or quit game?", 
        "1. Play Again",
        "2. Quit Game",
        nullptr};
    drawArtAtXY(-1, 16, endOption);
    int endWidth, endHeight;
    getWidthAndHeight((char**)endOption, &endWidth, &endHeight);
    const char* arrow[] = {"->", nullptr};
    drawArtAtXY((SCREEN_WIDTH - endWidth)/2 - 2, 17, arrow);
    OptionType endSelection = YES;

    init_terminal();
    set_terminal_mode(false); 
        while (true) {
        if (kbhit()) {
            char ch = my_getch();
            if (ch == 'w' || ch == 'W') {
                endSelection = static_cast<OptionType>((endSelection - 1 + 2) % 2);
            } else if (ch == 's' || ch == 'S') {
                endSelection = static_cast<OptionType>((endSelection + 1) % 2);
            } else if (ch == '\r' || ch == '\n') {
                break;
            }
            for (int i = 0; i < 2; i++) {
                gotoxy((SCREEN_WIDTH - endWidth)/2 - 2, 17 + i);
                if (i == endSelection) {
                    drawArtAtXY((SCREEN_WIDTH - endWidth)/2 - 2, 17 + i, arrow);
                } else {
                    const char* space[] = {"  ", nullptr};
                    drawArtAtXY((SCREEN_WIDTH - endWidth)/2 - 2, 17 + i, space);
                }
            }
        }
    }

    if (endSelection == YES) {
        GlobalgameState = STATE_TITLE;
    } else {
        GlobalgameState = STATE_EXIT;
    }
    restore_terminal();
}

int main() {

    GlobalgameState = STATE_TITLE;
    setcursortype(NOCURSOR);

    while(true) {
        switch (GlobalgameState)
        {
        case STATE_TITLE:
            drawStartScreen();
            handleStartScreenInput();
            break;
        case STATE_PLAYER_NAME_INPUT:
            drawPlayerNameInputScreen();
            handlePlayerNameInput();
            break;
        case STATE_PLAYER_LOAD:
            drawPlayerLoadScreen();
            handlePlayerLoadInput();
            break;
        case STATE_PLAYER_CREATION:
            drawCreateCharacterScreen();
            handleCreateCharacterInput();
            break;
        case STATE_ENEMY_CREATION:
            drawEnemyCreationScreen();
            handleEnemyCreationInput();
            break;
        case STATE_BATTLE:
            drawBattleScreen();
            handleBattleInput();
            break;
        case STATE_GAME_OVER:
            drawGameOverScreen();
            handleGameOverInput();
            break;
        case STATE_EXIT:
        default:
            return 0;
        }
    }
    return 0;
}




#if defined(__linux__) || defined(__APPLE__)

static struct termios original_tio;
static struct termios blocking_tio;
static struct termios non_blocking_tio;
static int peek_character = -1;

#endif


void init_terminal() {

#if defined(__linux__) || defined(__APPLE__)
    tcgetattr(STDIN_FILENO, &original_tio);
    blocking_tio = original_tio;
    blocking_tio.c_lflag &= ~(ICANON | ECHO);
    blocking_tio.c_cc[VMIN] = 1;
    blocking_tio.c_cc[VTIME] = 0;
    non_blocking_tio = original_tio;
    non_blocking_tio.c_lflag &= ~(ICANON | ECHO);
    non_blocking_tio.c_cc[VMIN] = 0;
    non_blocking_tio.c_cc[VTIME] = 0;
#endif
}

void restore_terminal() {
#if defined(__linux__) || defined(__APPLE__)
    tcsetattr(STDIN_FILENO, TCSANOW, &original_tio);
#endif
}

void set_terminal_mode(bool blocking) {
#if defined(__linux__) || defined(__APPLE__)
    if (blocking) {
        tcsetattr(STDIN_FILENO, TCSANOW, &blocking_tio);
    }
    else {
        tcsetattr(STDIN_FILENO, TCSANOW, &non_blocking_tio);
    }
#endif
}

int kbhit(void) {
#if defined(__linux__) || defined(__APPLE__)
    if (peek_character != -1) return 1;
    char ch;
    int nread = read(STDIN_FILENO, &ch, 1);
    if (nread == 1) {
        peek_character = ch;
        return 1;
    }
    return 0;
#elif _WIN32
    return _kbhit();
#endif
}

int my_getch(void) {
#if defined(__linux__) || defined(__APPLE__)
    char ch;
    if (peek_character != -1) {
        ch = peek_character;
        peek_character = -1;
        return ch;
    }
    int nread = read(STDIN_FILENO, &ch, 1);
    if (nread == 1) return ch;
    return -1;
 #elif _WIN32
	 return _getch();
#endif
}

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#elif defined(__linux__) || defined(__APPLE__)
    system("clear");
#endif
}

void savePlayerToFile(Character* player)
{
    std::ofstream outFile("player_data.bin", std::ios::binary);
    if (outFile.is_open()) {
        outFile.write(reinterpret_cast<const char*>(player), sizeof(Character));
        outFile.close();
    }
}

bool loadPlayerFromFile(Character* player)
{
    std::ifstream inFile("player_data.bin", std::ios::binary);
    if (inFile.is_open()) {
        inFile.read(reinterpret_cast<char*>(player), sizeof(Character));
        inFile.close();
        return true;
    }
    return false;
}
