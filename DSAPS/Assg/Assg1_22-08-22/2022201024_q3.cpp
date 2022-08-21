#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;

int calculateEnergy(int i, int j, int ***rgb, int W, int H){
    int deltaXsq, deltaYsq;

    //y gradient
    int j1=j-1, j2=j+1;
    if(j1<0) j1=W-1;
    if(j2==W) j2=0;

    int Rx = rgb[i][j2][0]-rgb[i][j1][0];
    int Gx = rgb[i][j2][1]-rgb[i][j1][1];
    int Bx = rgb[i][j2][2]-rgb[i][j1][2];

    deltaXsq = (Rx*Rx)+(Gx*Gx)+(Bx*Bx);

    //x gradient
    int i1=i-1, i2=i+1;
    if(i1<0) i1=H-1;
    if(i2==H) i2=0;

    int Ry = rgb[i2][j][0]-rgb[i1][j][0];
    int Gy = rgb[i2][j][1]-rgb[i1][j][1];
    int By = rgb[i2][j][2]-rgb[i1][j][2];

    deltaYsq = (Ry*Ry)+(Gy*Gy)+(By*By);
    return deltaXsq+deltaYsq;
}

int* getVerticalSeam(int** costMatrix, int H, int W){
    int* vSeam = new int[H];
    int idx=0;

    //find min element index in last row
    int minVal=costMatrix[H-1][0];
    for(int j=1; j<W; j++){
        if(costMatrix[H-1][j]<minVal){
            minVal=costMatrix[H-1][j];
            vSeam[idx]=j;
        }
    }

    for(int i=H-2; i>=0; i--){
        int j=vSeam[idx++];
        vSeam[idx]=j;
        minVal=costMatrix[i][j];
        if(j-1>=0 && costMatrix[i][j-1]<minVal){
            minVal=costMatrix[i][j-1];
            vSeam[idx]=j-1;
        }
        if(j+1<W && costMatrix[i][j+1]<minVal){
            minVal=costMatrix[i][j+1];
            vSeam[idx]=j+1;
        }
    }

    return vSeam;
}

int* getHorizontalSeam(int** costMatrix,  int H, int W){
    int* hSeam = new int[W];
    int idx=0;

    //find min element index in last row
    int minVal=costMatrix[0][W-1];
    for(int i=1; i<H; i++){
        if(costMatrix[i][W-1]<minVal){
            minVal=costMatrix[i][W-1];
            hSeam[idx]=i;
        }
    }

    for(int j=W-2; j>=0; j--){
        int i=hSeam[idx++];
        hSeam[idx]=i;
        minVal=costMatrix[i][j];
        if(i-1>=0 && costMatrix[i-1][j]<minVal){
            minVal=costMatrix[i-1][j];
            hSeam[idx]=i-1;
        }
        if(i+1<H && costMatrix[i+1][j]<minVal){
            minVal=costMatrix[i+1][j];
            hSeam[idx]=i+1;
        }
    }

    return hSeam;
}

void solve(int ***rgb, int H, int W, int C, int H_, int W_, int C_) {

    // Write your code here
    int **energyMatrix;
    energyMatrix = new int *[H];
    for(int i=0; i<H; i++) {
        energyMatrix[i] = new int [W];
        for(int j=0; j<W; j++){
            energyMatrix[i][j]=0;
        }
    }

    while(W>W_){

        //calcuating current energy matrix
        for(int i=0; i<H; i++){
            for(int j=0; j<W; j++){
                energyMatrix[i][j]=calculateEnergy(i, j, rgb, W, H);
            }
        }

        //calculate cost matrix
        for(int i=1; i<H; i++){
            for(int j=0; j<W; j++){
                int left=j-1, right=j+1;
                int val=2147483647;
                if(left>=0 && right<W){
                    val = min(energyMatrix[i][j]+energyMatrix[i-1][left], energyMatrix[i][j]+energyMatrix[i-1][right]);
                }else if(left<0 && right<W){
                    val=energyMatrix[i][j]+energyMatrix[i-1][right];
                }else if(right>=W && left>=0){
                    val=energyMatrix[i][j]+energyMatrix[i-1][left];
                }
                energyMatrix[i][j]=min(energyMatrix[i][j]+energyMatrix[i-1][j], val);
            }
        }

        //calculate vertical seam
        int* verSeam = getVerticalSeam(energyMatrix, H, W);

        //delete seam
        for(int i=H-1; i>=0; i--){
            //delete index rgb[i][verSeam[H-1-i]];
            for(int j=verSeam[H-1-i]; j<W-1; j++){
                rgb[i][j][0]=rgb[i][j+1][0];
                rgb[i][j][1]=rgb[i][j+1][1];
                rgb[i][j][2]=rgb[i][j+1][2];   
            }
        }

        W--;
    }

    while(H>H_){

        //calculate current energy matrix
        for(int i=0; i<H; i++){
            for(int j=0; j<W; j++){
                energyMatrix[i][j]=calculateEnergy(i, j, rgb, W, H);
            }
        }

        //calculate cost matrix
        for(int j=1; j<W; j++){
            for(int i=0; i<H; i++){
                int top=i-1, down=i+1;
                int val=2147483647;
                if(top>=0 && down<W){
                    val = min(energyMatrix[i][j]+energyMatrix[top][j-1], energyMatrix[i][j]+energyMatrix[down][j-1]);
                }else if(top<0 && down<W){
                    val=energyMatrix[i][j]+energyMatrix[down][j-1];
                }else if(down>=W && top>=0){
                    val=energyMatrix[i][j]+energyMatrix[top][j-1];
                }
                energyMatrix[i][j]=min(energyMatrix[i][j]+energyMatrix[i][j-1], val);
            }
        }

        //calculate horizontal seam
        int* horSeam = getHorizontalSeam(energyMatrix, H, W);

        //delete seam
        for(int j=W-1; j>=0; j--){
            //delete index rgb[i][verSeam[H-1-i]];
            for(int i=horSeam[W-1-j]; i<H-1; i++){
                rgb[i][j][0]=rgb[i+1][j][0];
                rgb[i][j][1]=rgb[i+1][j][1];
                rgb[i][j][2]=rgb[i+1][j][2];   
            }
        }
        H--;
    }

}

int main() {
    string ip_dir = "./data/input/";
    string ip_file = "rgb_in.txt";
    ifstream fin(ip_dir + ip_file);

    int H, W, C;
    fin >> H >> W >> C;

    int ***rgb;
    rgb = new int **[H];
    for(int i = 0; i < H; ++i) {
        rgb[i] = new int *[W];
        for(int j = 0; j < W; ++j) {
            rgb[i][j] = new int[C];
            for(int k = 0; k < C; ++k)
                fin >> rgb[i][j][k];
        }
    }
    fin.close();

    int H_, W_, C_;
    cout << "Enter new value for H (must be less than " << H << "): ";
    cin >> H_;
    cout << "Enter new value for W (must be less than " << W << "): ";
    cin >> W_;
    cout << '\n';
    C_ = C;

    solve(rgb, H, W, C, H_, W_, C_);

    string op_dir = "./data/output/";
    string op_file = "rgb_out.txt";
    ofstream fout(op_dir + op_file);
    
    fout << H_ << " " << W_ << " " << C_ << endl;
    for(int i = 0; i < H_; ++i) {
        for(int j = 0; j < W_; ++j) {
            for(int k = 0; k < C_; ++k) {
                fout << rgb[i][j][k] << " ";
            }
        }
        fout << '\n';
    }
    fout.close();

    return 0;
}