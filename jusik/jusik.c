#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

#define MAX_DAYS 30
#define MAX_INITIAL_PRICE 100
#define VARIATION_RANGE 15  // �ֽ� ���� ���� ����

int goodbad;
char* news() {
    int newsrandom;
    static char goodnews[5][100] = {
        "������ �ϳ´�. �������� �Ｚ���� �̱⳪?",
        "�������� �ż��� ���� �����ϳ�?",
        "���������� �ܱ� ���� ���ɼ��� ���δٰ�?",
        "������. ���� �����ִ� ��������",
        "������ �߱�����... ���������� ��"
    };

    static char badnews[5][100] = {
        "�� �Ұ� �ܾ簣 ��ġ�� ��������",
        "�߱��� ���� ��������",
        "\"�ϰ� ����\" �������� �߾� �ϴ� �������� ����",
        "�� �̻��� ������ڴ� ������ ��������",
        "����� �ٸ� ���� ���� ������ ����ģ ��������"
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

// �ֽ� ���� �����͸� �����ϴ� �Լ�
void generateStockPrices(float prices[], int days, float initialPrice) {
    prices[0] = initialPrice;
    for (int i = 1; i < days; i++) {
        float variation = (rand() % (VARIATION_RANGE * 200 + 1) - VARIATION_RANGE * 100) / 100.0;
        prices[i] = prices[i - 1] + variation;
        if (prices[i] < 0) prices[i] = 0;  // �ֽ� ������ ������ �� �� ����
    }
}

// �ܼ� Ŀ�� ��ġ�� �����ϴ� �Լ�
void setCursorPosition(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// �ֽ� ������ �׷����� ����ϴ� �Լ�
void drawStockGraph(float prices[], int days, int* balance, int* stocks) {
    float maxPrice = 0;
    // �ִ� �ְ��� ã��
    for (int i = 0; i < days; i++) {
        if (prices[i] > maxPrice) {
            maxPrice = prices[i];
        }
    }

    // �׷��� �׸���
    for (int i = 0; i < days; i++) {
        int height = (int)((prices[i] / maxPrice) * 10);  // �׷����� ���̸� 10���� ����
        char* current_news = news();
        setCursorPosition(70, i);  // ���� ��� ��ġ ����
        printf("�Ӻ�: %s\n", current_news);
        setCursorPosition(i * 2, 20 - height);  // Ŀ�� ��ġ ���� (x, y)
        for (int j = 0; j < height; j++) {
            printf("*");
            setCursorPosition(i * 2, 20 - height + j + 1);  // ���� �ٷ� �̵�
        }
        setCursorPosition(i * 2, 21);  // x ��ġ�� Ŀ�� �̵�
        printf(" %.2f", prices[i]);  // �� ���� �ְ��� ���

        // ���� �ֱ⸶�� �ŷ� �Է� �ޱ�
        setCursorPosition(0, 22);  // �Է� ��ġ ����
        printf("Press 'b' to buy or 's' to sell stocks: ");
        if (_kbhit()) {  // Ű �Է��� �ִ��� Ȯ��
            char choice = _getch();
            int quantity;
            if (choice == 'b') {  // ����ڰ� 'b'�� �Է��ϸ� �ֽ��� �ż�
                printf("\nHow many stocks do you want to buy? ");  // �ż��� �ֽ� ���� �����ϴ�
                scanf("%d", &quantity);
                if (*balance >= quantity * prices[i]) {  // �ܾ��� ������� Ȯ��
                    *stocks += quantity;  // ���� �ֽ� ���� �ż��� �ֽ��� ���մϴ�
                    *balance -= quantity * prices[i];  // �ܾ׿��� �ż��� �ֽ� ������ ���ϴ�
                    printf("Bought %d stocks. Balance: %d, Stocks: %d\n", quantity, *balance, *stocks);
                }
                else {
                    printf("Insufficient balance to buy %d stocks.\n", quantity);  // �ܾ��� ������ ���
                }
            }
            else if (choice == 's') {  // ����ڰ� 's'�� �Է��ϸ� �ֽ��� �ŵ�
                printf("\nHow many stocks do you want to sell? ");  // �ŵ��� �ֽ� ���� �����ϴ�
                scanf("%d", &quantity);
                if (*stocks >= quantity) {  // ������ �ֽ� ���� ������� Ȯ��
                    *stocks -= quantity;  // ���� �ֽ� ������ �ŵ��� �ֽ��� ���ϴ�
                    *balance += quantity * prices[i];  // �ܾ׿� �ŵ��� �ֽ� ������ ���մϴ�
                    printf("Sold %d stocks. Balance: %d, Stocks: %d\n", quantity, *balance, *stocks);
                }
                else {
                    printf("Insufficient stocks to sell %d stocks.\n", quantity);  // ������ �ֽ� ���� ������ ���
                }
            }

            // �ŷ� �� ��� ���
            Sleep(2000);

            // �ŷ� �� �޽��� �����
            setCursorPosition(0, 22);
            printf("                                                                    ");
            setCursorPosition(0, 23);
            printf("                                                                    ");
            setCursorPosition(0, 24);
            printf("                                                                    ");

            // ȭ�� ����
            setCursorPosition(0, 1);
            printf("�ڻ�: %d, �� ����: %d\n", *balance, *stocks);
        }

        Sleep(500);   // �� ĭ �׸� �� 0.5�� ���
    }
}

int main() {
    float prices[MAX_DAYS];
    srand(time(0));  // ���� �õ� ����

    int balance = 1000;  // �ʱ� �ڻ�
    int stocks = 0;      // ���� �ֽ� ��
    float initialPrice = (float)(rand() % MAX_INITIAL_PRICE) + (rand() % 100) / 100.0;  // �ʱ� �ֽ� ���� ����

    while (1) {
        generateStockPrices(prices, MAX_DAYS, initialPrice);

        if (goodbad == 1) {
            // �ֽ� ������ ������ ���, ������ �ֽ� ������ 1.05�� ���ݴϴ�.
            prices[MAX_DAYS - 1] *= 1.05;
        }
        else {
            // �ֽ� ������ �������� ���, ������ �ֽ� ������ 0.95�� ���ݴϴ�.
            prices[MAX_DAYS - 1] *= 0.98;
        }

        initialPrice = prices[MAX_DAYS - 1];  // ������ �ֽ� ������ ���� ó�� ������ ����

        system("cls");  // �ܼ� ȭ�� �����

        printf("�ֽ� ���� �׷���\n");
        printf("�ڻ�: %d, �� ����: %d\n", balance, stocks);
        printf("�ֽ� �̸� : ��������");
        drawStockGraph(prices, MAX_DAYS, &balance, &stocks);

        Sleep(1000);  // �׷����� ������ �׸� �� 1�� ���
    }

    return 0;
}
