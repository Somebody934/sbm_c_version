#include <iostream>
#include <vector>       //vektor
#include <algorithm>    // za copy
#include <fstream>

#define DIM 6
using namespace std;
/* for-ovi su neki uobicajni za c a neki uobicajniji za python */
/*** Plan mi je ovakav: Zelim da ubrzam pretragu da li je neki identitet zadovoljen pa sam predstavio operaciju
 * meet preko Kejlijeve tablice (a+b -> mat[a][b]). Identitete koje zelim ispitati su sledeci: ***/
/** Tran: [(z+y)+x)]+[(x+y)+z]=(x+y)+z                 -> mat[mat[mat[x][y]][z]][mat[mat[x][y]][z]]= mat[mat[x][y]][z]
 *  Comp: [(y+x)+(u+z)]+[(x+y)+(z+u)]=(x+y)+(z+u)      -> mat[mat[mat[y][x]][mat[u][z]]][mat[mat[x][y]][mat[z][u]]]=mat[mat[x][y]][mat[z][u]]
 *  Comm: (y+x)+(x+y)=x+y                              -> mat[mat[y][x]][mat[x][y]]=mat[x][y]
 *  **/

/**     Želja mi je da nadjem grupoid tako da važi Comm i Tran, a ne Comp**/


/** proverava da li grupoid zadovoljava tran **/
bool check_tran(const short *mat, bool print = false) {
    for (int i = 0; i < DIM * DIM; i++) {
        for (int j = 0; j < DIM * DIM; j++) {
            for (int k = 0; k < DIM * DIM; k++) {
                short x = *(mat + i);
                short y = *(mat + j);
                short z = *(mat + k);
                if (*(mat + DIM * (*(mat + DIM * (*(mat + DIM * z + y)) + x)) +
                      (*(mat + DIM * (*(mat + DIM * x + y)) + z))) != *(mat + DIM * (*(mat + DIM * x + y)) + z)) {
                    if (print)
                        cout << x << " " << y << " " << z << endl;
                    return false;
                }

            }
        }
    }
    return true;
}

/** proverava da li grupoid zadovoljava comp **/
bool check_comp(const short *mat, bool print = false) {
    for (int i = 0; i < DIM * DIM; i++) {
        for (int j = 0; j < DIM * DIM; j++) {
            for (int k = 0; k < DIM * DIM; k++) {
                for (int l = 0; l < DIM * DIM; l++) {
                    int x = *(mat + i);
                    int y = *(mat + j);
                    int z = *(mat + k);
                    int u = *(mat + l);
                    if (*(mat + DIM * (*(mat + DIM * (*(mat + DIM * y + x)) + (*(mat + DIM * u + z)))) +
                          *(mat + DIM * (*(mat + DIM * x + y)) + (*(mat + DIM * z + u)))) !=
                        *(mat + DIM * (*(mat + DIM * x + y)) + (*(mat + DIM * z + u)))) {
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

/** proverava da li grupoid zadovoljava comm **/
bool check_comm(const short *mat, bool print = false) {
    for (int i = 0; i < DIM * DIM; i++) {
        for (int j = 0; j < DIM * DIM; j++) {

            int x = *(mat + i);
            int y = *(mat + j);
            if (*(mat + DIM * (*(mat + DIM * y + x)) + (*(mat + DIM * x + y))) != *(mat + DIM * x + y)) {
                if (print)
                    cout << x << " " << y << endl;
                return false;
            }


        }
    }
    return true;
}

/**popunjavanje matrice**/
void fill_matrix(short *mat, vector<short int> filler) {
    int br = 0;
    for (int i = 0; i < DIM * DIM; i++) {
        if (*(mat + i) == -1) {
            *(mat + i) = filler[br];
            br++;
        }
    }
}

ofstream file;
int brojac = 0;
/** nzm kako ovo radi tj nisam gledao kako ovo radi, jer je meni da bitno da barem nekako radi**/
/** Link sa kog sam uzeo kod: https://stackoverflow.com/a/63763630**/

/** Proverava da li je matrica ona kakva nam odgovara tj da li vazi tran, comm i !comp **/
/* matrix - originalna matrica koja treba da se dopuni, filler - dopuna*/
void check_matrix(short matrix[][DIM], const vector<short> &filler) {
    short matrica[DIM][DIM];
    copy(&matrix[0][0], &matrix[0][0] + DIM * DIM - 1, &matrica[0][0]);
    short *mat_ptr = &matrica[0][0];
    fill_matrix(mat_ptr, filler);
    if ((check_comm(mat_ptr) && check_comp(mat_ptr) && !(check_tran(mat_ptr)))) {
        file.open("maybe.txt", ios_base::app);
//        file << 1;
//        for (auto &re: matrica) {
//            for (int j: re) {
//                file << j;
//            }
//            file << endl;
//        }
        file << endl;
        file.close();
    }

}

/* pravljenje svih dopuna matrice preko rekurzije nekako (nisam ja pisao, samo sam promenio da kad se generise jedna dopuna da se ona odmah proveri*/
void CartesianRecurse(vector<short> stack,
                      vector<vector<short int>> sequences, int index, short matrix[][DIM]) {
    vector<short int> sequence = sequences[index];
    for (short i: sequence) {
        stack.push_back(i);
        if (index == 0) {
            if (++brojac % 1000 == 0) {
                cout << brojac << endl;
            }
            check_matrix(matrix, stack);
        } else
            CartesianRecurse(stack, sequences, index - 1, matrix);
        stack.pop_back();
    }
}



int main() {
    /* ovi nizovi su odabrani da kao predstavljaju tilda klase sem o koji cine svi elementi*/
    vector<short int> x = {3, 4};
    vector<short int> y = {2, 5};
    vector<short int> o = {0, 1, 2, 3, 4, 5};
    /* sequences - niz skupova od kojih zelimo uredenu ntorku da ubacimo u niz */
    /* mora se pisati obrnuto od uobicajnog citanja (od dole desno ka levo i gore)*/
    vector<vector<short int>> sequences = /**/{y, x, y, y, y, x, o, x, x, o, y, o, o, x, y, x, y, y, x,
                                               o};/*/{o, y, x};/**/
/*originalna matrica je ono sto kao znamo*/
    short o_matrica[DIM][DIM] = {
            {0,  1,  3,  -1, -1, -1},
            {0,  1,  5,  -1, -1, -1},
            {4,  5,  2,  -1, -1, 5},
            {-1, -1, -1,  3,  4,  -1},
            {-1, -1, -1, 3,  4,  -1},
            {-1, -1, 2,  -1, -1, 5}
    };
    int m1count = 0;
    for (auto &i: o_matrica) {
        for (short j: i) {
            if (j == -1) {
                m1count++;
            }
        }
    }
    if (m1count == sequences.size() and m1count > 0) {
        vector<short> stack;
        cout << "start" << endl;
        CartesianRecurse(stack, sequences, sequences.size() - 1, o_matrica);
    }

    cout << brojac << endl;
    return 0;
}