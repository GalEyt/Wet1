#include "AVLTree.h"
#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <iostream>

void unitTest4()
{
    AVLTree<int, int> *root = new AVLTree<int, int>;
    std::random_device r;
    std::default_random_engine e1(r());
    std::uniform_int_distribution<int> uniform_dist(1, 1000);
    int tmp = uniform_dist(e1);
    int order[100];
    for (int i = 0; i < 200; i++)
    {
        order[i] = uniform_dist(e1);
        try
        {
            root = root->insert(1, order[i]);
        }
        catch (Exist)
        {
            
        }
        catch (...)
        {
            
        }
        
        //std::cout << "\n \n";
    }
    root->printTreeInOrder();
    //std::cout << "\n \n";
    // for (int i = 0; i < 100; i++)
    // {
    //     std::cout << order[i] << " ";
    // }
    
}

void unitTest5()
{
    AVLTree<int, int> *root = new AVLTree<int, int>;

    int arr[] = {833,180,61,252,496,231,331,379,408,945,756,619,994,579,828,722,919,870,261,689,545,314,778,972,919,769,129,933,507,72,675,971,522,724,314,613,222,883,114,591,57,711,860,888,98,665,263,759,686,390,654,975,452,151,777,949,690,533,77,218,288,177,216,930,199,619,26,49,347,912,940,673,574,761,583,937,577,56,272,566,950,899,234,302,337,853,513,840,886,179,711,129,262,103,745,428,748,383,250,429};
    for (int i = 0; i < sizeof(arr)/sizeof(int); i++)
    {
        if(i==50){
            std::cout<<" \n";
        }
        try
        {
            root->insert(1, arr[i]);
        }
        catch (Exist)
        {
            std::cout << "exist";
        }
        catch (...)
        {
            std::cout << "failed";
        }
           std::cout << "\n \n" << "iteration: "<< i << "tree:";
        root->printTreeInOrder();
    }
    //root->printTreeInOrder();
}

void unitTest6(){
    AVLTree<int, int> *root = new AVLTree<int, int>;
    root->insert(1, 8);
    root->insert(1, 11);
    root->insert(1, 5);
    root->insert(1, 12);
    root->insert(1, 10);
    root->print2D(root);
    root = root->remove(11);
    std::cout << "\n---------- \n";
    root->print2D(root);
}

void unitTest7(){
    AVLTree<int, int> *root = new AVLTree<int, int>;
    root->insert(1, 10);
    root->insert(1, 5);
    root->insert(1, 15);
    root->insert(1, 4);
    root->insert(1, 6);
    root->insert(1, 14);
    root->insert(1, 16);
    root->insert(1, 13);
    root->print2D(root);
    root = root->remove(10);
    std::cout << "\n---------- \n";
    root->print2D(root);
}

void unitTest8(){
    AVLTree<int, int> *root = new AVLTree<int, int>;
    root = root->insert(1, 1);
    root = root->insert(1, 2);
    root = root->insert(1, 3);
    root->print2D(root);
}

int main()
{
    unitTest4();
}