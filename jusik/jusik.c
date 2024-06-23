#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

#define MAX_DAYS 30
#define MAX_INITIAL_PRICE 100
#define VARIATION_RANGE 15  // 주식 가격 변동 범위

int goodbad;
char* news() {
    int newsrandom;
    static char goodnews[5][100] = {
        "인턴이 일냈다. 선린전자 삼성전자 이기나?",
        "선린전자 신소재 개발 성공하나?",
        "선린전자의 외국 진출 가능성이 보인다고?",
        "대혁신. 드디어 보여주는 선린전자",
        "이제는 중국까지... 선린전자의 힘"
    };

    static char badnews[5][100] = {
        "소 잃고 외양간 고치는 선린전자",
        "중국이 만들어낸 설린전자",
        "\"니가 뭔데\" 인종차별 발언 하는 선린전자 사장",
        "더 이상을 못만들겠다 선언한 선린전자",
        "남들과 다른 길을 가다 절벽을 마주친 선린전자"
    };

    newsrandom = rand() % 5;
    goodbad = rand() % 2;
    if (goodbad == 0) {
        return goodnews[newsrandom];
    }
    else {
        return badnews[newsrandom];
    }
}

// 주식 가격 데이터를 생성하는 함수
void generateStockPrices(float prices[], int days, float initialPrice) {
    prices[0] = initialPrice;
    for (int i = 1; i < days; i++) {
        float variation = (rand() % (VARIATION_RANGE * 200 + 1) - VARIATION_RANGE * 100) / 100.0;
        prices[i] = prices[i - 1] + variation;
        if (prices[i] < 0) prices[i] = 0;  // 주식 가격은 음수가 될 수 없음
    }
}

// 콘솔 커서 위치를 설정하는 함수
void setCursorPosition(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// 주식 가격을 그래프로 출력하는 함수
void drawStockGraph(float prices[], int days, int* balance, int* stocks) {
    float maxPrice = 0;
    // 최대 주가를 찾음
    for (int i = 0; i < days; i++) {
        if (prices[i] > maxPrice) {
            maxPrice = prices[i];
        }
    }

    // 그래프 그리기
    for (int i = 0; i < days; i++) {
        int height = (int)((prices[i] / maxPrice) * 10);  // 그래프의 높이를 10으로 조정
        char* current_news = news();
        setCursorPosition(70, i);  // 뉴스 출력 위치 설정
        printf("속보: %s\n", current_news);
        setCursorPosition(i * 2, 20 - height);  // 커서 위치 설정 (x, y)
        for (int j = 0; j < height; j++) {
            printf("*");
            setCursorPosition(i * 2, 20 - height + j + 1);  // 다음 줄로 이동
        }
        setCursorPosition(i * 2, 21);  // x 위치로 커서 이동
        printf(" %.2f", prices[i]);  // 각 날의 주가를 출력

        // 일정 주기마다 거래 입력 받기
        setCursorPosition(0, 22);  // 입력 위치 설정
        printf("Press 'b' to buy or 's' to sell stocks: ");
        if (_kbhit()) {  // 키 입력이 있는지 확인
            char choice = _getch();
            int quantity;
            if (choice == 'b') {  // 사용자가 'b'를 입력하면 주식을 매수
                printf("\nHow many stocks do you want to buy? ");  // 매수할 주식 수를 묻습니다
                scanf("%d", &quantity);
                if (*balance >= quantity * prices[i]) {  // 잔액이 충분한지 확인
                    *stocks += quantity;  // 보유 주식 수에 매수한 주식을 더합니다
                    *balance -= quantity * prices[i];  // 잔액에서 매수한 주식 가격을 뺍니다
                    printf("Bought %d stocks. Balance: %d, Stocks: %d\n", quantity, *balance, *stocks);
                }
                else {
                    printf("Insufficient balance to buy %d stocks.\n", quantity);  // 잔액이 부족할 경우
                }
            }
            else if (choice == 's') {  // 사용자가 's'를 입력하면 주식을 매도
                printf("\nHow many stocks do you want to sell? ");  // 매도할 주식 수를 묻습니다
                scanf("%d", &quantity);
                if (*stocks >= quantity) {  // 보유한 주식 수가 충분한지 확인
                    *stocks -= quantity;  // 보유 주식 수에서 매도한 주식을 뺍니다
                    *balance += quantity * prices[i];  // 잔액에 매도한 주식 가격을 더합니다
                    printf("Sold %d stocks. Balance: %d, Stocks: %d\n", quantity, *balance, *stocks);
                }
                else {
                    printf("Insufficient stocks to sell %d stocks.\n", quantity);  // 보유한 주식 수가 부족할 경우
                }
            }

            // 거래 후 잠시 대기
            Sleep(2000);

            // 거래 후 메시지 지우기
            setCursorPosition(0, 22);
            printf("                                                                    ");
            setCursorPosition(0, 23);
            printf("                                                                    ");
            setCursorPosition(0, 24);
            printf("                                                                    ");

            // 화면 갱신
            setCursorPosition(0, 1);
            printf("자산: %d, 주 개수: %d\n", *balance, *stocks);
        }

        Sleep(500);   // 한 칸 그린 후 0.5초 대기
    }
}

int main() {
    float prices[MAX_DAYS];
    srand(time(0));  // 난수 시드 설정

    int balance = 1000;  // 초기 자산
    int stocks = 0;      // 보유 주식 수
    float initialPrice = (float)(rand() % MAX_INITIAL_PRICE) + (rand() % 100) / 100.0;  // 초기 주식 가격 설정

    while (1) {
        generateStockPrices(prices, MAX_DAYS, initialPrice);

        if (goodbad == 1) {
            // 주식 가격이 오르는 경우, 마지막 주식 가격을 1.05배 해줍니다.
            prices[MAX_DAYS - 1] *= 1.05;
        }
        else {
            // 주식 가격이 내려가는 경우, 마지막 주식 가격을 0.95배 해줍니다.
            prices[MAX_DAYS - 1] *= 0.98;
        }

        initialPrice = prices[MAX_DAYS - 1];  // 마지막 주식 가격을 다음 처음 값으로 설정

        system("cls");  // 콘솔 화면 지우기

        printf("주식 가격 그래프\n");
        printf("자산: %d, 주 개수: %d\n", balance, stocks);
        printf("주식 이름 : 선린전자");
        drawStockGraph(prices, MAX_DAYS, &balance, &stocks);

        Sleep(1000);  // 그래프를 완전히 그린 후 1초 대기
    }

    return 0;
}
