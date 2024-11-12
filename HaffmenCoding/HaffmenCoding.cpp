#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <queue>
#include <string>
#include <Windows.h>

using namespace std;

struct Node
{
    char ch;
    int freq;
    Node* left, * right; // Правый и левый потомок
};

Node* createNode(char ch, int freq, Node* left, Node* right) {
    Node *node = new Node;

    node->ch = ch;
    node->freq = freq;
    node->left = left;
    node->right = right;

    return node;
}

struct comparator {
    bool operator()(Node* l, Node* r) {
        // Больший приоритет имеет объект с меньшей частотой. ??? Как работают компараторы в приоритетных очередях.
        return l->freq > r->freq;
    }
};


Node* build_huffman_tree(string text, map<char, int> &chars_freqs) {

    // Создаем частотную таблицу алфавита нашего сообщения.
    for (char ch : text) {
        chars_freqs[ch] += 1;
    }

    // Создаем приоритетную очередь с нашим компаратором, для изменения функции приоритета в ней
    priority_queue<Node*, vector<Node*>, comparator> pr_queue;

    // Добавляем в очередь листья дерева Хаффмена
    for (auto pair : chars_freqs) {
        pr_queue.push(createNode(pair.first, pair.second, nullptr, nullptr));
    }

    // Строим дерево, пока в очереди не останется последний элемент (корень дерева)
    while (pr_queue.size() != 1) {

        // Достаем из очереди наиболее преоритетный элемент (с наименьшей частотой)
        Node *left = pr_queue.top(); pr_queue.pop();
        Node *right = pr_queue.top(); pr_queue.pop();

        // Создаем новый узел и помещаем его в очередь
        pr_queue.push(createNode('\0', left->freq + right->freq, left, right));
    }

    // Достаем последний оставшийся узел (корень) в переменную
    Node* root = pr_queue.top(); pr_queue.pop();

    return root;

}


void encode(Node *root, string current_code, map<char, string> &chars_hcodes) {

    // Првоеряем, дошли ли мы до листа
    if (!root->left && !root->right) {
        chars_hcodes[root->ch] = current_code;
        current_code = "";
        return;
    }

    encode(root->left, current_code + "0", chars_hcodes);
    encode(root->right, current_code + "1", chars_hcodes);

}

void decode(Node *root, int &i, string encoded_text, string &decoded_text) {

        // Првоеряем, дошли ли мы до листа
        if (!root->left && !root->right) {
            decoded_text += root->ch;
            return;
        }

        i++; // int i = -1 в начале
        if (encoded_text[i] == '0') {
            decode(root->left, i, encoded_text, decoded_text);
        }
        else {
            decode(root->right, i, encoded_text, decoded_text);
        }

}


void printTitle() {
    cout << "	" << " _   _          __   __                          " << endl;
    cout << "	" << "| | | |        / _| / _|                         " << endl;
    cout << "	" << "| |_| | _   _ | |_ | |_  _ __ ___    __ _  _ __  " << endl;
    cout << "	" << "|  _  || | | ||  _||  _|| '_ ` _ \\  / _` || '_ \\ " << endl;
    cout << "	" << "| | | || |_| || |  | |  | | | | | || (_| || | | |" << endl;
    cout << "	" << "\\_| |_/ \\__,_||_|  |_|  |_| |_| |_| \\__,_||_| |_|" << endl;
    cout << "	" << "c o d i n g " << endl << endl;
}


int main()
{
    setlocale(LC_ALL, "Russian");
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    printTitle();

    int m;
    cout << "Выберите способ ввода текста для кодирования\n";
    cout << "1 - ввод с клавиатуры\n";
    cout << "2 - чтение из Текст.txt\n";
    cout << "Ввод: "; cin >> m;

    string text_to_encode;
    if (m == 1) {
        cout << "Введите текст для кодирования: ";
        cin.ignore();
        getline(cin, text_to_encode);
    }
    else if (m == 2) {
        // Читаем текст для кодирования из файла в переменную
        string text_file_path = "Текст.txt";

        text_to_encode = ""; string temp;
        fstream file(text_file_path);
        if (file.is_open()) {
            while (!file.eof()) {
                getline(file, temp);
                text_to_encode = text_to_encode + temp + "\n";
            }
            file.close();
        }
        else {
            cout << "Возникла ошибка при открытии файла с текстом.\n";
        }
    }
    else {
        cout << "Некорректный ввод\n";
        return 0;
    }

    // Строим дерево Хаффмена и сохраняем его корень в переменную
    map<char, int> chars_freqs;
    Node* root = build_huffman_tree(text_to_encode, chars_freqs);

    // Получаем словарь char - Huffman code
    map<char, string> chars_hcodes;
    string current_code = "";
    encode(root, current_code, chars_hcodes);
    // Получаем закодированный текст
    string encoded_text = "";
    for (char ch : text_to_encode) { encoded_text += chars_hcodes[ch]; }

    // Получаем раскодированный текст
    string decoded_text = ""; int i = -1;
    while (i < (int)encoded_text.size() - 2) {
        decode(root, i, encoded_text, decoded_text);
    }

    // Выводим все на экран

    cout << "\nТекст для кодирования: " << text_to_encode << "\n\n";

    cout << "Символ\tЧастота\t  Код" << "\n";
    for (auto pair : chars_hcodes) {
        cout << pair.first << "\t" << chars_freqs[pair.first] << "\t" << "  " << pair.second << "\n";
    }

    cout << "\nЗакодированный текст: " << encoded_text << "\n";

    cout << "Раскодированный текст: " << decoded_text << "\n\n";

}

