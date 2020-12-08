//==============================================================================
//                               WordSplitter
//==============================================================================

//==============================================================================
// Библиотеки и константы
//==============================================================================

#include  <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h> // Для установки национальных параметров
#include  <wchar.h> // Описание функций для работы с "толстыми" (двухбайтовыми) символами
#include <wctype.h>
#include  <errno.h> // Проверка ошибок с файлами

// Максимальная длина строки с текстом
#define N 1000

//==============================================================================
// Объявление функций
//==============================================================================

void work_with_user();
void work_with_files(char** argv);
wchar_t* split_string(wchar_t* string);
wchar_t* split_word(wchar_t* word);
wchar_t* sample_word (wchar_t* word);

//==============================================================================

int main(int argc, char** argv)
{
    setlocale(LC_CTYPE, "ru_RU.UTF-8");

    switch(argc)
    {
        case 1:
            work_with_user();  // Ввод с клавиатуры, вывод в терминал (work_with_user)
            break;
        case 2:
            printf("Введите имена файлов для чтения и записи, иначе вводите с клавиатуры!");
            break;
        default:
            work_with_files(argv); // Чтение из файла argv[1], запись в файл argv[2] (work_with_files)
            break;
    }

    return 0;
}

//==============================================================================
// Реализация функций
//==============================================================================

//------------------------------------------------------------------------------
// Функции работы с пользователем
//------------------------------------------------------------------------------

// Работа с клавиатуры
void work_with_user()
{
    printf("Введите текст, в котором хотите расставить переносы:\n");

    wchar_t* string = calloc(N + 1, sizeof(wchar_t));

    while (fgetws(string, N, stdin) != NULL)
        fputws(split_string(string), stdout);

    free(string);
}

// Работа с файлами
void work_with_files(char** argv)
{
    FILE *fin, *fout;

    fin  = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    if (errno)
        printf("ERROR");

    wchar_t* string = calloc(N + 1, sizeof(wchar_t));

    while (fgetws(string, N, fin) != NULL)
        fputws(split_string(string), fout);

    free(string);
    fclose(fin);
    fclose(fout);
}

//------------------------------------------------------------------------------
// Функции алгоритма расстановки переносов
//------------------------------------------------------------------------------

// Функция расстановки переносов в строке
wchar_t* split_string(wchar_t* string)
{
    wchar_t* find;
    wchar_t* part = string;
    wchar_t* splitted_string = (wchar_t*)calloc(wcslen(string) * 3, sizeof(wchar_t));
    wmemset(splitted_string, L'\0', wcslen(splitted_string) * sizeof(wchar_t));

    for (int i = 0; (find = wcsstr(part, L" ")) != NULL; i++)
    {
        find[0] = L'\0';
        wcscat(splitted_string, split_word(part));
        wcscat(splitted_string, L" ");
        part = find + 1;
    }

    wcscat(splitted_string, split_word(part));

    return splitted_string;
}

// Функция расстановки переносов в слове
wchar_t* split_word(wchar_t* word)
{
    wchar_t* sampled_word = (wchar_t*)calloc(wcslen(word) + 1, sizeof(wchar_t));
    wchar_t* splitted_word = (wchar_t*)calloc(wcslen(word) * 3, sizeof(wchar_t));
    wmemset(splitted_word, L'\0', wcslen(splitted_word) * sizeof(wchar_t));

    sampled_word = sample_word(word);

    return sample_word(word);
}

// Преобразует слово к шаблону
wchar_t* sample_word(wchar_t* word)
{
    wchar_t* sampled_word = (wchar_t*)calloc(wcslen(word) + 1, sizeof(wchar_t));

    for (int i = 0; i < wcslen(word); i++)
    {
        if (iswalpha(word[i]))
        {
            switch(word[i])
            {
                case L'A': case L'Е': case L'Ё': case L'И': case L'О': case L'У':
                case L'Э': case L'Ю': case L'Я':
                    wcscat(sampled_word, L"Г");
                    break;
                case L'а': case L'е': case L'ё': case L'и': case L'о': case L'у':
                case L'э': case L'ю': case L'я':
                    wcscat(sampled_word, L"г");
                    break;
                case L'Ъ': case L'ъ': case L'Ы': case L'ы': case L'Ь': case L'ь':
                case L'Й': case L'й':
                    wcsncat(sampled_word, word + i, 1);
                    break;
                default:
                    if (iswupper(word[i]))
                        wcscat(sampled_word, L"С");
                    else
                        wcscat(sampled_word, L"с");
                    break;
            }
        }
        else
            wcsncat(sampled_word, word + i, 1);
    }

    return sampled_word;
}
