#include<iostream>
#include<vector>
#include<limits>
#include<cmath>
#include"Node.h"
using namespace std;


class LoserTree
{
public:
    int size;
    int leaf;
    vector<Node> tree;
    int max;

    LoserTree(int n, vector<int> input)
    {
        size = 2 * n - 1; leaf = n;

        tree.resize(size);
        for (int i = 0; i < n; i++)
        {
            tree[i + n - 1] = Node(input[i], i + n - 1);
        }
    }

    Node build()
    {
        int pass = log2(size + 1);
        vector<Node> win(leaf);
        for (int i = 0; i < leaf; i++)
        {
            win[i] = tree[leaf - 1 + i];
        }
        for (int i = 1; i < pass; i++)
        {
            int num_count = leaf / i;
            int new_count = num_count / 2;
            vector<Node> temp(new_count);
            int start = pow(2, pass - 1 - i) - 1;
            for (int j = 0; j < new_count; j++)
            {
                tree[start + j] = win[2 * j].value < win[2 * j + 1].value ? win[2 * j + 1] : win[2 * j];
                temp[j] = win[2 * j].value < win[2 * j + 1].value ? win[2 * j] : win[2 * j + 1];
            }
            win.resize(new_count);
            for (int j = 0; j < new_count; j++)
            {
                win[j] = temp[j];
            }
        }
        max = win[0].value;
        return win[0];
    }

    Node update(int index, int num)
    {
        tree[index].value = num;
        if (num < max)
        {
            tree[index].vaild = false;
        }
        else
        {
            tree[index].vaild = true;
        }

        int compare = (index - 1) / 2;
        Node win = tree[index];

        while (compare > 0)
        {
            if (win.vaild == false && tree[compare].vaild == true)
            {
                Node temp = win;
                win = tree[compare];
                tree[compare] = temp;
            }
            if (win.vaild == true && tree[compare].vaild == true)
            {
                if (win.value > tree[compare].value)
                {
                    Node temp = win;
                    win = tree[compare];
                    tree[compare] = temp;
                }
            }
            compare = (compare - 1) / 2;
        }
        compare = 0;
        if (win.vaild == false && tree[compare].vaild == true)
        {
            Node temp = win;
            win = tree[compare];
            tree[compare] = temp;
        }
        if (win.vaild == true && tree[compare].vaild == true)
        {
            if (win.value > tree[compare].value)
            {
                Node temp = win;
                win = tree[compare];
                tree[compare] = temp;
            }
        }

        max = win.value;
        return win;

    }

    bool checkVaild()
    {
        for (int i = leaf - 1; i < size; i++)
        {
            if (tree[i].vaild == true)
                return true;
        }
        return false;
    }

    void turntrue()
    {
        for (int i = leaf - 1; i < size; i++)
        {
            tree[i].vaild = true;
        }
    }

};
