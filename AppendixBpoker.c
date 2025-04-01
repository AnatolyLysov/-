// Программа-пример из приложения B
// Руководства по С для новичков, 3-е издание
// Файл AppendixBpoker.c
/* Программа представляет собой игру «Покер
с обменом». Пользователи могут играть так часто,
Приложение Б
354
как захотят, делая ставки величиной от 1 до 5.
Пользователям выдается по 5 карт, после чего
пользователь должен решить, какие карты оставить,
а какие заменить. После этого происходит оценка вновь
выданных карт — и пользователь получает вознаграждение
в зависимости от ценности имеющихся у него на руках
карт. Далее программа отображает на экране новый
банкролл пользователя, и ему дается возможность
продолжить игру */
// Заголовочные файлы
#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include <stdlib.h>
// Объявление двух констант для определения, имеется
// ли у пользователя на руках флеш или стрит.
#define FALSE 0
#define TRUE 1
// Прототипы функций
void printGreeting();
int getBet();
char getSuit(int suit);
char getRank(int rank);
void getFirstHand(int cardRank[], int cardSuit[]);
void getFinalHand(int cardRank[], int cardSuit[], int
finalRank[],
int finalSuit[], int
ranksinHand[],
int suitsinHand[]);
int analyzeHand(int ranksinHand[], int suitsinHand[]);
main()
{
int bet;
int bank = 100;
int i;
int cardRank[5]; // Одно из 13 значений (Туз-Король)
int cardSuit[5]; // Одно из 4 значений (для Пик,
// Бубен, Черв, Треф)
Программа «Покер с обменом»
355
int finalRank[5];
int finalSuit[5];
int ranksinHand[13]; // Используется для оценки
// последней раздачи
int suitsinHand[4]; // Используется для оценки
// последней раздачи
int winnings;
time_t t;
char suit, rank, stillPlay;
// Функция вызывается вне цикла do...while,
// так как приветствие нужно отобразить
// только однажды, тогда как все остальное
// в функции main будет запущено
// несколько раз, в зависимости от того, сколько
// раз пользователь захочет сыграть.
printGreeting();
// Цикл запускается каждый раз, когда пользователь
// запрашивает раздачу новой партии карт
do {
bet = getBet();
srand(time(&t));
getFirstHand(cardRank, cardSuit);
printf("Ваши пять карт: \n");
for (i = 0; i < 5; i++)
{
suit = getSuit(cardSuit[i]);
rank = getRank(cardRank[i]);
printf("Карта №%d: %c%c\n", i+1, rank, suit);
}
// Эти два массива используются, чтобы
// провести оценку карт, выданных
// пользователю. Однако они должны быть
// обнулены, если пользователь затребует
// несколько раздач.
for (i=0; i < 4; i++)
{
Приложение Б
356
suitsinHand[i] = 0;
}
for (i=0; i < 13; i++)
{
ranksinHand[i] = 0;
}
getFinalHand(cardRank, cardSuit, finalRank,
finalSuit, ranksinHand, suitsinHand);
printf("Ваша последняя партия карт: \n");
for (i = 0; i < 5; i++)
{
suit = getSuit(finalSuit[i]);
rank = getRank(finalRank[i]);
printf("Карта №%d: %c%c\n", i+1, rank, suit);
}
winnings = analyzeHand(ranksinHand,suitsinHand);
printf("Вы выиграли %d!\n", bet*winnings);
bank = bank - bet + (bet*winnings);
printf("\nВ вашем банке сейчас %d.\n", bank);
printf("\nХотите сыграть ещё раз? ");
scanf(" %c", &stillPlay);
} while (toupper(stillPlay) == 'Y');
return;
}
/****************************************************/
// Распечатать быстрое приветствие, а также рассказать
// пользователям о ценности различных выигрышных
// комбинаций
void printGreeting()
{
printf("*************************************\n");
printf("\n\n\tДобро пожаловать в казино для
новичков!\n\n");
printf("\tДом видео покера с обменом\n\n");
printf("*************************************\n");
printf("Правила таковы:\n");
Программа «Покер с обменом»
357
printf("Ваш начальный баланс 100 кредитов, вы
должны сделать ");
printf("ставку в размере от 1 до 5 кредитов.\n");
printf("Вам выдано 5 карт, вы должны выбрать,
какие ");
printf("карты оставить, а какие - ");
printf("сбросить.\n");
printf("Вам нужно собрать наилучшую из возможных
комбинаций.\n");
printf("\nДалее приведена таблица выигрышей
(в расчете на ");
printf("ставку в 1 кредит):");
printf("\nПара\t\t\t\t1 кредит");
printf("\nДве пары\t\t\t2 кредита");
printf("\nТри карты одного типа\t\t\t3 кредита ");
printf("\nСтрит\t\t\t4 кредита");
printf("\nФлэш\t\t\t\t5 кредитов");
printf("\nФулхауз\t\t\t8 кредитов ");
printf("\nЧетыре карты одного типа\t\t\t10
кредитов");
printf("\nФлэш-рояль\t\t\t20 кредитов");
printf("\n\nРазвлекайтесь!!\n\n");
}
// Функция для выдачи первых пяти карт
void getFirstHand(int cardRank[], int cardSuit[])
{
int i,j;
int cardDup;
for (i=0; i < 5; i++)
{
cardDup = 0;
do {
// Один из 13 номиналов карты (2-10, В, Д, K, Т)
cardRank[i] = (rand() % 13);
// Одна из четырех мастей карты
// (пики, бубны, червы, трефы)
cardSuit[i] = (rand() % 4);
Приложение Б
358
// Цикл, гарантирующий уникальность каждой карты
for (j=0; j < i; j++)
{
if ((cardRank[i] == cardRank[j]) &&
(cardSuit[i] == cardSuit[j]))
{
cardDup = 1;
}
}
} while (cardDup == 1);
}
}
// Функция, заменяющая целочисленный номер масти на
// букву, представляющую эту масть
char getSuit(int suit)
{
switch (suit)
{
case 0:
return('п');
case 1:
return('б');
case 2:
return('ч');
case 3:
return('т');
}
}
// Функция, заменяющая целочисленный номер номинала
// карты на букву, представляющую этот номинал
char getRank(int rank)
{
switch (rank)
{
case 0:
return('Т');
Программа «Покер с обменом»
359
case 1:
return('2');
case 2:
return('3');
case 3:
return('4');
case 4:
return('5');
case 5:
return('6');
case 6:
return('7');
case 7:
return('8');
case 8:
return('9');
case 9:
return('10');
case 10:
return('В');
case 11:
return('Д');
case 12:
return('K');
}
}
// Функция для получения ставки пользователя (от 1 до 5)
int getBet()
{
int bet;
do //Будет повторяться, пока пользователь не
введет 0-5
{
printf("Сколько вы хотите поставить? (Введите
число от ");
printf("1 до 5 или 0 для выхода из игры): ");
Приложение Б
360
scanf(" %d", &bet);
if (bet >= 1 && bet <= 5)
{
return(bet);
}
else if (bet == 0)
{
exit(1);
}
else
{
printf("\n\nПожалуйста введите ставку от 1
до 5 или ");
printf("0 для выхода из игры.\n");
}
} while ((bet < 0) || (bet > 5));
}
// Последняя функция проводит оценку последней
// выданной игроку партии карт
int analyzeHand(int ranksinHand[], int suitsinHand[])
{
int num_consec = 0;
int i, rank, suit;
int straight = FALSE;
int flush = FALSE;
int four = FALSE;
int three = FALSE;
int pairs = 0;
for (suit = 0; suit < 4; suit++)
if (suitsinHand[suit] == 5)
flush = TRUE;
rank = 0;
while (ranksinHand[rank] == 0)
rank++;
for (; rank < 13 && ranksinHand[rank]; rank++)
num_consec++;
Программа «Покер с обменом»
361
if (num_consec == 5) {
straight = TRUE;
}
for (rank = 0; rank < 13; rank++) {
if (ranksinHand[rank] == 4)
four = TRUE;
if (ranksinHand[rank] == 3)
three = TRUE;
if (ranksinHand[rank] == 2)
pairs++;
}
if (straight && flush) {
printf("Флеш-рояль\n\n");
return (20);
}
else if (four) {
printf("Четыре одного типа\n\n");
return (10);
}
else if (three && pairs == 1) {
printf("Фуллхаус\n\n");
return (8);
}
else if (flush) {
printf("Флеш\n\n");
return (5);
}
else if (straight) {
printf("Стрит\n\n");
return (4);
}
else if (three) {
printf("Три одного типа\n\n");
return (3);
}
else if (pairs == 2) {
Приложение Б
362
printf("Две пары\n\n");
return (2);
}
else if (pairs == 1) {
printf("Пара\n\n");
return (1);
}
else {
printf("Старшая карта\n\n");
return (0);
}
}
// Функция просматривает все пять карт первой партии
// и спрашивает пользователя, хочет ли он оставить
// карту. Если нет, то карта пользователя заменяется.
void getFinalHand(int cardRank[], int cardSuit[], int
finalRank[],
int finalSuit[], int ranksinHand[],
int suitsinHand[])
{
int i, j, cardDup;
char suit, rank, ans;
for (i=0; i < 5; i++)
{
suit = getSuit(cardSuit[i]);
rank = getRank(cardRank[i]);
printf("Хотите сохранить карту №%d: %c%c?",
i+1, rank, suit);
printf("\nПожалуйста, ответьте (Y/N): ");
scanf(" %c", &ans);
if (toupper(ans) == 'Y')
{
finalRank[i] = cardRank[i];
finalSuit[i] = cardSuit[i];
ranksinHand[finalRank[i]]++;
suitsinHand[finalSuit[i]]++;
Программа «Покер с обменом»
363
continue;
}
else if (toupper(ans) == 'N')
{
cardDup = 0;
do {
cardDup = 0;
finalRank[i] = (rand() % 13);
finalSuit[i] = (rand() % 4);
// Сравнить новую карту с 5 картами первой
// партии для во избежание повторов
for (j=0; j < 5; j++)
{
if ((finalRank[i] == cardRank[j]) &&
(finalSuit[i] == cardSuit[j]))
{
cardDup = 1;
}
}
// Далее, сопоставить новую карту
// с только что
// выданными во избежание повторов
for (j=0; j < i; j++)
{
if ((finalRank[i] == finalRank[j]) &&
(finalSuit[i] == finalSuit[j]))
{
cardDup = 1;
}
}
} while (cardDup == 1);
ranksinHand[finalRank[i]]++;
suitsinHand[finalSuit[i]]++;
}
}
}
