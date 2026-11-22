
#include <iostream>
using namespace std;

struct CNode
{
    CNode(int value)
    {
        v = value;
        height = 1;
        nodes[0] = nodes[1] = nullptr;
    }

    int v;
    int height;
    CNode* nodes[2];
};

class CAVL
{
public:
    CAVL();
    ~CAVL();

    bool ins(int x);
    bool find(CNode* n, int x);

    void print();
    void inorder(CNode* n);

private:
    CNode* root;

    // Altura y balance
    int getHeight(CNode* n);
    int getBalance(CNode* n); //diferencia de alturas
    void updateHeight(CNode* n);

    // Las 4 rotaciones AVL
    CNode* rotateLL(CNode* n);
    CNode* rotateRR(CNode* n);
    CNode* rotateLR(CNode* n);
    CNode* rotateRL(CNode* n);

    // Balancear
    CNode* rebalance(CNode* n);

    // Insertar 
    CNode* insertAVL(CNode* n, int x);

    // Liberar memoria
    void clear(CNode* n);
};

CAVL::CAVL()
{
    root = nullptr;
}

CAVL::~CAVL()
{
    clear(root);
}

int CAVL::getHeight(CNode* n)
{
    if (!n) return 0;
    return n->height;
}

void CAVL::updateHeight(CNode* n)
{
    if (!n) return;

    int left = getHeight(n->nodes[0]);
    int right = getHeight(n->nodes[1]);

    if (left > right) {
      n->height = left + 1;
    }
    else {
        n->height = right + 1;
    }
}

int CAVL::getBalance(CNode* n)
{
    if (!n) return 0;
    int AlturaIzq = getHeight(n->nodes[0]);
    int AlturaDer = getHeight(n->nodes[1]);

    return AlturaIzq - AlturaDer;
}

CNode* CAVL::rotateLL(CNode* n)
{
    CNode* temp = n->nodes[0];
    CNode* copy = temp->nodes[1];

    temp->nodes[1] = n;
    n->nodes[0] = copy;

    updateHeight(n);
    updateHeight(temp);
    return temp;
}

CNode* CAVL::rotateRR(CNode* n)
{
    CNode* temp = n->nodes[1];
    CNode* copy = temp->nodes[0];

    temp->nodes[0] = n;
    n->nodes[1] = copy;

    updateHeight(n);
    updateHeight(temp);
    return temp;
}

CNode* CAVL::rotateLR(CNode* n)
{
    n->nodes[0] = rotateRR(n->nodes[0]);
    return rotateLL(n);
}

CNode* CAVL::rotateRL(CNode* n)
{
    CNode* temp = n;
    CNode* temp1 = temp->nodes[1];
    CNode* temp2 = temp1->nodes[0];

    CNode* copy1 = temp2->nodes[0];
    CNode* copy2 = temp2->nodes[1];

    temp->nodes[1] = temp2;
    temp2->nodes[1] = temp1;
    temp1->nodes[0] = copy2;

    temp2->nodes[0] = temp;
    temp->nodes[1] = copy1;

    updateHeight(temp);
    updateHeight(temp1);
    updateHeight(temp2);
    return temp2;

}

CNode* CAVL::rebalance(CNode* n)
{
    updateHeight(n);
    int b = getBalance(n);

    if (b > 1) //izquierda, como se si es LR o LL
    {
        if (getBalance(n->nodes[0]) >= 0) { //LL si es positivo o 0
            return rotateLL(n);
        }
        else {
            return rotateLR(n); //LR cuando el balance es negativo
        }
    }

    if (b < -1)
    { //derecha, RL o RR
        if (getBalance(n->nodes[1]) <= 0) {
            return rotateRR(n);
        }
        else {
            return rotateRL(n);
        }

    }
    return n;
}

CNode* CAVL::insertAVL(CNode* n, int x) {
    if (!n) {
        CNode* nuevo = new CNode(x);
        return nuevo;
    }

    if (x < n->v) {
        n->nodes[0] = insertAVL(n->nodes[0], x);
    }
    else if (x > n->v) {
        n->nodes[1] = insertAVL(n->nodes[1], x);
    }
    else { //valor repetido
        return n;
    }
    return rebalance(n);
}

bool CAVL::find(CNode* n, int x) //comprueba si existe
{
    if (!n) return false;

    if (x == n->v) {
        return true;
    }

    if (x < n->v) {
        return find(n->nodes[0], x);
    }
    else {
        return find(n->nodes[1], x);
    }
}


bool CAVL::ins(int x) {
    if (find(root, x)) {
        return false; 
    }

    root = insertAVL(root, x); //insertar desde el root recursivamente
    return true;
}

void CAVL::inorder(CNode* n) {
    if (!n) return;
    inorder(n->nodes[0]);
    cout << n->v << " " ;
    inorder(n->nodes[1]);
}

void CAVL::print()
{
    cout << "AVL Inorder: ";
    inorder(root);
    cout << endl;
}

void CAVL::clear(CNode* n)
{
    if (!n) return;
    clear(n->nodes[0]);
    clear(n->nodes[1]);
    delete n;
}

int main()
{
    CAVL arbol;
    arbol.ins(50);
    arbol.ins(20);
    arbol.ins(10);
    arbol.ins(30);
    arbol.ins(80);
    arbol.print();
  

    return 0;
}