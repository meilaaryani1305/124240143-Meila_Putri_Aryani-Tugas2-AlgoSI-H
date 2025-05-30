#include <iostream>
#include <string>
using namespace std;

struct Book
{
    string title;
    string author;
    int year;
};

struct TreeNode
{
    Book data;
    TreeNode *left;
    TreeNode *right;
};

struct StackNode
{
    Book data;
    string action;
    StackNode *next;
};

TreeNode *root = nullptr;
StackNode *top = nullptr;

TreeNode *buatNode(Book book)
{
    TreeNode *newNode = new TreeNode;
    newNode->data = book;
    newNode->left = nullptr;
    newNode->right = nullptr;
    return newNode;
}

TreeNode *tambahBuku(TreeNode *node, Book book)
{
    if (node == nullptr)
    {
        return buatNode(book);
    }
    if (book.title < node->data.title)
    {
        node->left = tambahBuku(node->left, book);
    }
    else if (book.title > node->data.title)
    {
        node->right = tambahBuku(node->right, book);
    }
    return node;
}

void tampilkanBuku(TreeNode *node)
{
    if (node == nullptr)
        return;
    tampilkanBuku(node->left);
    cout << "Judul: " << node->data.title
         << ", Pengarang: " << node->data.author
         << ", Tahun: " << node->data.year << endl;
    tampilkanBuku(node->right);
}

TreeNode *hapusBuku(TreeNode *root, string title, Book &deletedBook, bool &found)
{
    if (root == nullptr)
        return nullptr;

    if (title < root->data.title)
    {
        root->left = hapusBuku(root->left, title, deletedBook, found);
    }
    else if (title > root->data.title)
    {
        root->right = hapusBuku(root->right, title, deletedBook, found);
    }
    else
    {
        found = true;
        deletedBook = root->data;
        if (root->left == nullptr)
        {
            TreeNode *temp = root->right;
            delete root;
            return temp;
        }
        else if (root->right == nullptr)
        {
            TreeNode *temp = root->left;
            delete root;
            return temp;
        }
        else
        {
            TreeNode *temp = root->right;
            while (temp->left != nullptr)
                temp = temp->left;
            root->data = temp->data;
            root->right = hapusBuku(root->right, temp->data.title, deletedBook, found);
        }
    }
    return root;
}

void tambahTumpukan(Book book, string action)
{
    StackNode *newNode = new StackNode;
    newNode->data = book;
    newNode->action = action;
    newNode->next = top;
    top = newNode;
}

bool keluarTumpukan(Book &book, string &action)
{
    if (top == nullptr)
        return false;
    book = top->data;
    action = top->action;
    StackNode *temp = top;
    top = top->next;
    delete temp;
    return true;
}

void undoLastAction()
{
    Book book;
    string action;
    if (!keluarTumpukan(book, action))
    {
        cout << "Tidak ada aksi yang bisa di-undo.\n";
        return;
    }

    if (action == "tambah")
    {
        bool found = false;
        root = hapusBuku(root, book.title, book, found);
        cout << "Undo tambah: Buku \"" << book.title << "\" dihapus.\n";
    }
    else if (action == "delete")
    {
        root = tambahBuku(root, book);
        cout << "Undo hapus: Buku \"" << book.title << "\" dikembalikan.\n";
    }
}

int main()
{
    int pilihan;
    do
    {
        cout << "\n=== MENU BUKU ===\n";
        cout << "1. Tambah Buku\n";
        cout << "2. Tampilkan Buku (A-Z)\n";
        cout << "3. Hapus Buku\n";
        cout << "4. Undo Aksi Terakhir\n";
        cout << "5. Keluar\n";
        cout << "Pilih: ";
        cin >> pilihan;
        cin.ignore();

        if (pilihan == 1)
        {
            Book buku;
            cout << "Judul: ";
            getline(cin, buku.title);
            cout << "Pengarang: ";
            getline(cin, buku.author);
            cout << "Tahun terbit: ";
            cin >> buku.year;
            cin.ignore();
            root = tambahBuku(root, buku);
            tambahTumpukan(buku, "tambah");
        }
        else if (pilihan == 2)
        {
            tampilkanBuku(root);
        }
        else if (pilihan == 3)
        {
            string judul;
            cout << "Masukkan judul buku yang akan dihapus: ";
            getline(cin, judul);
            Book deletedBook;
            bool found = false;
            root = hapusBuku(root, judul, deletedBook, found);
            if (found)
            {
                tambahTumpukan(deletedBook, "delete");
                cout << "Buku berhasil dihapus.\n";
            }
            else
            {
                cout << "Buku tidak ditemukan.\n";
            }
        }
        else if (pilihan == 4)
        {
            undoLastAction();
        }
        else if (pilihan == 5)
        {
            cout << "Keluar dari program.\n";
        }
        else
        {
            cout << "Pilihan tidak valid.\n";
        }
    } while (pilihan != 5);

    return 0;
}