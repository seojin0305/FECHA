/*#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <algorithm>
#include <stdexcept>*/
#include <bits/stdc++.h>

#define UGN 25
#define GMA 3125
#define RHO_1 5
#define RHO_2 10
#define TAU 3130

using namespace std;

enum Operator {
    AND,
    XOR,
    OR,
    NOT
};

class HEScheme {
private:
    int pk[TAU + 1], sk;
    int _1;

public:
    void keyGen(int lb) {
        int r[TAU + 1];
        int maxIdx = 0;
        int tmp;
        
        srand((int)time(NULL));
        
        sk = (rand() * 2 + 1) % ((int) pow(2, UGN));
        
        int qMax = (int) pow(2, GMA) / sk - 1, rMax = (int) pow(2, RHO_1);
        
        do {
            for (int i = 0; i <= TAU; i++) {
                r[i] = rand() % (2 * rMax - 2) - (rMax - 1);
                pk[i] = sk * (rand() % qMax) + r[i];
            }
            
            for (int i = 0; i <= TAU; i++) {
                maxIdx = (pk[i] > pk[maxIdx]) ? i : maxIdx;
            }
            
            tmp = pk[0];
            pk[0] = pk[maxIdx];
            pk[maxIdx] = tmp;
        } while (pk[0] % 2 != 1 || r[maxIdx] % 2 != 0);
        
        _1 = encrypt(1);
    }

    int encrypt(int m) {
        vector<int> S;
        srand((int)time(NULL));
        
        for (int i = 1; i <= TAU; i++) {
            auto tmp = rand() % 200;
            if (tmp == 0) S.push_back(i);
        }
        
        auto gmaP = (rand() % ((int)pow(2, RHO_2) + 1)) - (int)pow(2, RHO_2);
        long long c = (long long)m + (long long)gmaP * 2;
        
        for (int i = 0; i < S.size(); i++) {c += 2 * pk[S[i]]; c % pk[0];}
        
        return c % pk[0];
    }
    
    int decrypt(int c) {
        return ((c % sk) % 2 + 2) % 2;
    }
    
    int evaluate(Operator C, int c1, int c2 = 1) {
        switch (C) {
        case AND:
            return c1 * c2;
            break;
        case XOR:
            return c1 + c2;
            break;
        case OR:
            return c1 * c2 + c1 + c2;
            break;
        case NOT:
            return c1 + _1;
            break;
        default:
            throw invalid_argument("Invalid Operation");
        }
    }
    
    void printKey() {
        printf("public key : ");
        
        for (int i = 0; i <= TAU; i++) printf("%d ", pk[i]);
        printf("\n");
        
        printf("secure key : %d\n", sk);
    }
}; 

int main() {
    HEScheme hES;
    
    hES.keyGen(5);
    int n, m, digit_n[100], digit_m[100], ans = 0, p_n = 0, p_m = 0, c_n[100], c_m[100], r[100], p = hES.encrypt(0);
    
    scanf("%d %d", &n, &m);
    
    while (n != 0) {
        digit_n[++p_n] = n % 2;
        n /= 2;
    }
    
    while (m != 0) {
        digit_m[++p_m] = m % 2;
        m /= 2;
    }
    
    for (int i = 1; i <= p_n; i++) {
        c_n[i] = hES.encrypt(digit_n[i]);
        c_m[i] = hES.encrypt(digit_m[i]);
        
        r[i] = hES.decrypt(hES.evaluate(XOR, p, hES.evaluate(XOR, c_n[i], c_m[i])));
        p = hES.evaluate(XOR, hES.evaluate(AND, c_n[i], c_m[i]), hES.evaluate(AND, p, hES.evaluate(XOR, c_n[i], c_m[i])));
        printf("%d\n", hES.decrypt(p));
    }
    
    r[p_n + 1] = hES.decrypt(p);
    
    for (int i = 1; i <= p_n + 1; i++) {
        ans += r[i] * (int)pow(2, i - 1);
    }
    
    printf("%d", ans);
    return 0;
}
