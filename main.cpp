#include <iostream>
#include <vector>       //vektor
#include <algorithm>    // za copy
#include <fstream>

using namespace std;
/* for-ovi su neki uobicajni za c a neki uobicajniji za python */
/*** Plan mi je ovakav: Zelim da ubrzam pretragu da li je neki identitet zadovoljen pa sam predstavio operaciju
 * meet preko Kejlijeve tablice (a+b -> mat[a][b]). Identitete koje zelim ispitati su sledeci: ***/
/** Tran: [(z+y)+x)]+[(x+y)+z]=(x+y)+z                 -> mat[mat[mat[x][y]][z]][mat[mat[x][y]][z]]= mat[mat[x][y]][z]
 *  Comp: [(y+x)+(u+z)]+[(x+y)+(z+u)]=(x+y)+(z+u)      -> mat[mat[mat[y][x]][mat[u][z]]][mat[mat[x][y]][mat[z][u]]]=mat[mat[x][y]][mat[z][u]]
 *  Comm: (y+x)+(x+y)=x+y                              -> mat[mat[y][x]][mat[x][y]]=mat[x][y]
 *  **/

/**     Želja mi je da nadjem grupoid tako da važi Comm i Tran, a ne Comp**/
/**TODO Umesto da napravim dekartov proizvod pa onda da proveravam, treba da cim se konstruise element proizvoda da ga odmah proverim**/
bool check_tran(int mat[][6], int dim, bool print = false) {
    for (int i = 0; i < dim * dim; i++) {
        for (int j = 0; j < dim * dim; j++) {
            for (int k = 0; k < dim * dim; k++) {
                int x = *(&mat[0][0] + i);
                int y = *(&mat[0][0] + j);
                int z = *(&mat[0][0] + k);
                if (mat[mat[mat[x][y]][z]][mat[mat[x][y]][z]] != mat[mat[x][y]][z]) {
                    if (print)
                        cout << x << " " << y << " " << z << endl;
                    return false;
                }

            }
        }
    }
    return true;
}

bool check_comp(int mat[][6], int dim, bool print = false) {
    for (int i = 0; i < dim * dim; i++) {
        for (int j = 0; j < dim * dim; j++) {
            for (int k = 0; k < dim * dim; k++) {
                for (int l = 0; l < dim * dim; l++) {
                    int x = *(&mat[0][0] + i);
                    int y = *(&mat[0][0] + j);
                    int z = *(&mat[0][0] + k);
                    int u = *(&mat[0][0] + l);
                    if (mat[mat[mat[y][x]][mat[u][z]]][mat[mat[x][y]][mat[z][u]]] != mat[mat[x][y]][mat[z][u]]) {
                        if (print)
                            cout << x << " " << y << " " << z << " " << u << endl;
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

bool check_comm(int mat[][6], int dim, bool print = false) {
    for (int i = 0; i < dim * dim; i++) {
        for (int j = 0; j < dim * dim; j++) {

            int x = *(&mat[0][0] + i);
            int y = *(&mat[0][0] + j);
            if (mat[mat[y][x]][mat[x][y]] != mat[x][y]) {
                if (print)
                    cout << x << " " << y << endl;
                return false;
            }


        }
    }
    return true;
}

/**popunjavanje matrice**/
void fill_matrix(int mat[][6], int dim, vector<short int> filler) {
    int br = 0;
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            if (mat[i][j] == -1) {
                mat[i][j] = filler[br];
                br++;
            }
        }
    }
}


/** nzm kako ovo radi tj nisam gledao kako ovo radi, jer je meni da bitno da barem nekako radi**/
/** Link sa kog sam uzeo kod: https://stackoverflow.com/a/63763630**/

void CartesianRecurse(vector<vector<short int>> &accum, vector<short> stack,
                      vector<vector<short int>> sequences, int index) {
    vector<short int> sequence = sequences[index];
    for (int i: sequence) {
        stack.push_back(i);
        if (index == 0)
            accum.push_back(stack);
        else
            CartesianRecurse(accum, stack, sequences, index - 1);
        stack.pop_back();
    }
}

vector<vector<short int>> CartesianProduct(vector<vector<short int>> sequences) {
    vector<vector<short int>> accum;
    vector<short> stack;
    if (sequences.size() > 0)
        CartesianRecurse(accum, stack, sequences, sequences.size() - 1);
    return accum;
}


int main() {
    ofstream file;
    /* ovi nizovi su odabrani da kao predstavljaju tilda klase sem o koji cine svi elementi*/
    vector<short int> x = {4, 5};
    vector<short int> y = {3, 6};
    vector<short int> o = {1, 2, 3, 4, 5, 6};
    /* sequences - niz skupova od kojih zelimo uredenu ntorku da ubacimo u niz */
    /* mora se pisati obrnuto od uobicajnog citanja (od dole desno ka levo i gore)*/
    vector<vector<short int>> sequences = /**/{y, x, y, y, y, x, o, x, x, y, o, o, x, y, x, y, y, x, o};/*/{x, y, x};*/
    vector<vector<short int>> res = CartesianProduct(sequences);
    cout << "start";
    int brojac = 0;
    // /**/čitanje
//    for (auto &re: res) {
//        for (int j: re)
//            cout << j << " ";
//        cout << endl;
//    }
    /**dimenzija matrice**/
    const int DIM = 6;
    /*originalna matrica je ono sto kao znamo*/
    int o_matrica[DIM][DIM] = {
//            {0, 1, 3, -1, -1, -1},
//            {0, 1, 5, 3,  4,  3},
//            {4, 5, 2, 4,  2,  5},
//            {1, 3, 4, 3,  4,  5},
//            {4, 4, 6, 3,  4,  3},
//            {3, 4, 2, 6,  0,  5}
            {0,  1,  3,  -1, -1, -1},
            {0,  1,  5,  -1, -1, -1},
            {4,  5,  2,  -1, -1, 5},
            {-1, -1, -1, 3,  4,  -1},
            {-1, -1, -1, 3,  4,  -1},
            {-1, -1, 2,  -1, -1, 5}
    };
    /* imamo originalnu matricu i kopiju koju popunjavamo (-1 su polja za popunjavanje*/
    int matrica[DIM][DIM];
    for (auto ree: res) {

        if (++brojac % 1000 == 0)
            cout << brojac << endl;
        /*kopiranje matrice*/
        copy(&o_matrica[0][0], &o_matrica[0][0] + DIM * DIM, &matrica[0][0]);

        fill_matrix(matrica, DIM, ree);
        check_comm(matrica, 6);
        if (check_comm(matrica, 6) && check_tran(matrica, 6) && !(check_comp(matrica, 6))) {
            file.open("maybe.txt");
            for (auto &re: matrica) {
                for (int j: re) {
                    file << j;
                }
                file << endl;
            }
            file << endl;
            file.close();
        }
//        for (auto &re: matrica) {
//            for (int j: re) {
//                cout << j;
//            }
//            cout << endl;
//        }
//        cout << endl;

    }


    return 0;
}