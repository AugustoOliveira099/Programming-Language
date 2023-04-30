#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <cctype>
using namespace std;

const int MAX=512;
const int S_MAX=30;


struct Pixel
{
    int R;
    int G;
    int B;
};


struct Img
{
    Pixel M[MAX][MAX];
    int nl;
    int nc;
    int lixo;
};

void menu_cout(int& Menu)
{

    cout << "SELECIONE O MODO DE EDICAO" << endl;
    cout << endl;
    cout << "1 - Efeito preto e branco" << endl;
    cout << "2 - Efeito preto e branco com mascara" << endl;
    cout << "3 - Inverter imagem verticalmente" << endl;
    cout << "4 - Cortar imagem" << endl;
    cout << "5 - Equalizar histograma" << endl;
    cout << "6 - Sair" << endl;
    cout << endl;

    char linha[32];
    cin.getline(linha, 32);
    if(isdigit(linha[0])){
        Menu = linha[0] - '0';
        // '0' = 48
        // '1' = 49
        // '1' - '0' = 1
    }else
    {
        cout << "Digite apenas numeros" << endl;
        Menu = 0;
    }
    

}

void Faz_mascara(Img& mascara, Img img_in)
{
    int i, j;
    mascara.nl = img_in.nl;
    mascara.nc = img_in.nc;
    mascara.lixo = img_in.lixo;
    for(i=0; i < MAX; i++)
    {
        for(j=0; j < MAX; j++)
        {
            if(j < (img_in.nc/2))
            {
                mascara.M[i][j].R = 0;
                mascara.M[i][j].G = 0;
                mascara.M[i][j].B = 0;
            }
            else
            {
                mascara.M[i][j].R = 255;
                mascara.M[i][j].G = 255;
                mascara.M[i][j].B = 255;
            }
        }
    }

}


void binariza_mascara(Img img_in, Img& img_out, Img mascara, int limiar)
{
    int i, j;
    float media;
    img_out.nl = img_in.nl;
    img_out.nc = img_in.nc;
    img_out.lixo = img_in.lixo;
    for(i=0; i< mascara.nl; i++)
    {
        for(j=0; j < mascara.nc ; j++)
        {
            if(mascara.M[i][j].R == 0 && mascara.M[i][j].G ==0 && mascara.M[i][j].B ==0)
            {
                img_out.M[i][j] = img_in.M[i][j];
            }
            else
            {
                media=(img_in.M[i][j].R + img_in.M[i][j].G + img_in.M[i][j].B)/3;
                if(media>=limiar)
                {
                    img_out.M[i][j].R = 255;
                    img_out.M[i][j].G = 255;
                    img_out.M[i][j].B = 255;
                }
                else
                {
                    img_out.M[i][j].R = 0;
                    img_out.M[i][j].G = 0;
                    img_out.M[i][j].B = 0;
                }
            }
        }
    }

}

int somatorio (int v[], int r)
{

    int i, soma=0;
    for(i=0; i<=r; i++)
    {
        soma+=v[i];
    }

    return soma;

}

void equaliza_histograma(Img img_in, Img& img_out)
{

    int n=256;
    int v_R[n], v_G[n], v_B[n], i, j, r;
    float total_pixel=img_in.nl*img_in.nc;
    int s_R[n], s_G[n], s_B[n];

    img_out.nl=img_in.nl;
    img_out.nc=img_in.nc;
    img_out.lixo=img_in.lixo;

    for(i=0; i<n; i++)
    {
        v_R[i]=0;
        v_G[i]=0;
        v_B[i]=0;
    }

    for(r=0; r<n; r++)
    {
        for(i=0; i<img_in.nl; i++)
        {
            for(j=0; j<img_in.nc; j++)
            {
                if(img_in.M[i][j].R==r)
                {
                    v_R[r]++;
                }
                if(img_in.M[i][j].G==r)
                {
                    v_G[r]++;
                }
                if(img_in.M[i][j].B==r)
                {
                    v_B[r]++;
                }
            }
        }
    }

    for(r=0; r<n; r++)
    {
        s_R[r]= (255.0/total_pixel) * somatorio (v_R, r);
        s_G[r]= (255.0/total_pixel) * somatorio (v_G, r);
        s_B[r]= (255.0/total_pixel) * somatorio (v_B, r);
    }

    for(r=0; r<n; r++)
    {
        for(i=0; i<img_in.nl; i++)
        {
            for(j=0; j<img_in.nc; j++)
            {
                if(img_in.M[i][j].R==r)
                {
                    img_out.M[i][j].R=s_R[r];
                }
                if(img_in.M[i][j].G==r)
                {
                    img_out.M[i][j].G=s_G[r];
                }
                if(img_in.M[i][j].B==r)
                {
                    img_out.M[i][j].B=s_B[r];
                }
            }
        }
    }

}

void corta (Img img_in, Img& img_out, int linhaCorte, int colunaCorte, int altura, int largura)
{

    int i, j;
    img_out.nl = img_in.nl;
    img_out.nc = img_in.nc;
    img_out.lixo = img_in.lixo;
    for (i=linhaCorte; i<altura; i++)
    {
        for(j=colunaCorte; j<largura; j++)
        {

            img_out.M[i][j].R = img_in.M[i][j].R;
            img_out.M[i][j].G = img_in.M[i][j].G;
            img_out.M[i][j].B = img_in.M[i][j].B;
        }
    }


}

void binariza (Img img_in, Img& img_out, int limiar)
{

    int i, j;
    float media;

    img_out.nc = img_in.nc;
    img_out.nl = img_in.nl;
    img_out.lixo = img_in.lixo;


    for(i=0; i<img_in.nl; i++)
    {
        for(j=0; j<img_in.nc; j++)
        {
            media=(img_in.M[i][j].R + img_in.M[i][j].G + img_in.M[i][j].B)/3;
            if(media>=limiar)
            {
                img_out.M[i][j].R = 255;
                img_out.M[i][j].G = 255;
                img_out.M[i][j].B = 255;
            }
            else
            {
                img_out.M[i][j].R = 0;
                img_out.M[i][j].G = 0;
                img_out.M[i][j].B = 0;
            }

        }

    }

}
void inverte(Img img_in, Img& img_out)
{
    int i, j;
    img_out.nl = img_in.nl;
    img_out.nc = img_in.nc;
    img_out.lixo = img_in.lixo;
    for (i=0; i<img_in.nl; i++)
    {
        for(j=0; j<img_in.nc; j++)
        {

            img_out.M[i][j].R = img_in.M[(img_in.nl-1)-i][j].R;
            img_out.M[i][j].G = img_in.M[(img_in.nl-1)-i][j].G;
            img_out.M[i][j].B = img_in.M[(img_in.nl-1)-i][j].B;
        }
    }

}

bool abre_img (char nome[], Img& img)
{
    fstream arq;
    char ppm[5] = ".ppm";
    int i, j;
    char p3[4];
    arq.open(strcat(nome,ppm));

    arq >> p3;

    if(!arq.is_open() || strcmp(p3, "P3")!=0)
    {
        cout << "ARQUIVO INVALIDO" << endl;
        arq.close();
        return false;
    }

    arq >> img.nc;
    arq >> img.nl;
    arq >> img.lixo;


    for (i=0; i<img.nl; i++)
    {
        for (j=0; j<img.nc; j++)
        {

            arq >> img.M[i][j].R;
            arq >> img.M[i][j].G;
            arq >> img.M[i][j].B;
        }
    }
    arq.close();


    return true;
}

bool salva_img (char nome[], Img img)
{

    ofstream arq;
    int i, j;
    char ppm [] = ".ppm";
    char p3[] = "P3";
    arq.open(strcat(nome, ppm));
    if(!arq.is_open())
    {
        return false;
    }
    arq << p3 << endl;
    arq << img.nc << " " << img.nl << endl;
    arq << img.lixo << endl;


    for (i=0; i<img.nl && i < MAX; i++)
    {
        for (j=0; j<img.nc && j < MAX; j++)
        {
            arq << img.M[i][j].R << " ";
            arq << img.M[i][j].G << " ";
            arq << img.M[i][j].B << " ";
        }
        arq << endl;
    }
    arq.close();


    return true;
}
void reseta(Img& img)
{
    // RESETANDO IMAGEM DE SAIDA
    img.nl = 0;
    img.nc = 0;
    img.lixo = 0;
    for (int i=0; i<MAX; i++)
    {
        for(int j=0; j<MAX; j++)
        {

            img.M[i][j].R = 0;
            img.M[i][j].G = 0;
            img.M[i][j].B = 0;
        }
    }

}


int main()
{

    char nome_arq[S_MAX], novo_nome[S_MAX];
    Img img;
    Img img_out;
    Img mascara;
    int limiar;
    int linhaCorte, colunaCorte, alturaCorte, larguraCorte;
    int Menu;

    menu_cout(Menu);

    while (Menu != 6)
    {

        switch (Menu)
        {

        case 1: // Binariza

            cout << "1 - Digite o nome do arquivo (nao eh necessario fornecer o tipo. Ex.: .ppm): \n";
            cin.getline (nome_arq, S_MAX);

            if (abre_img(nome_arq, img))
            {
                cout << endl;
                cout << "entre com o limiar" << endl;
                cin >> limiar;
                cout << endl;
                binariza(img, img_out, limiar);
                cout << "entre com o nome do novo arquivo" << endl;
                cin.ignore();
                cin.getline(novo_nome, S_MAX);
                salva_img(novo_nome, img_out);
                cout << endl;
                cout << "Imagem pronta." << endl;
                cout << endl;
                cout << "Deseja continuar?" << endl;
                cout << endl;
                menu_cout(Menu);

            }
            else
            {
                cout << "\nEste arquivo esta indiponivel" << endl;
                cout << "Deseja fazer outra operacao?" << endl;
                cout << endl;
                menu_cout(Menu);

            }


            break;

        case 2: //Binariza com máscara

            cout << "2 - Digite o nome do arquivo (nao eh necessario fornecer o tipo. Ex.: .ppm): \n";
            cin.getline (nome_arq, S_MAX);

            if (abre_img(nome_arq, img))
            {
                cout << endl;
                cout << "entre com o limiar" << endl;
                cin >> limiar;
                cout << endl;
                Faz_mascara(mascara, img);
                binariza_mascara(img, img_out, mascara, limiar);
                cout << "entre com o nome do novo arquivo" << endl;
                cin.ignore();
                cin.getline(novo_nome, S_MAX);
                salva_img(novo_nome, img_out);
                cout << endl;
                cout << "Imagem pronta." << endl;
                cout << endl;
                cout << "Deseja continuar?" << endl;
                cout << endl;
                menu_cout(Menu);

            }
            else
            {
                cout << "\nEste arquivo esta indiponivel" << endl;
                cout << "Deseja fazer outra operacao?" << endl;
                cout << endl;
                menu_cout(Menu);

            }

            break;

        case 3: // Inverte Imagem

            cout << "3 - Digite o nome do arquivo (nao eh necessario fornecer o tipo. Ex.: .ppm): \n";
            cin.getline (nome_arq, S_MAX);

            if (abre_img(nome_arq, img))
            {
                cout << endl;
                inverte(img, img_out);
                cout << "entre com o nome do novo arquivo" << endl;
                cin.getline(novo_nome, S_MAX);
                salva_img(novo_nome, img_out);
                cout << endl;
                cout << "Imagem pronta." << endl;
                cout << endl;
                cout << "Deseja continuar?" << endl;
                cout << endl;
                menu_cout(Menu);

            }
            else
            {
                cout << "\nEste arquivo esta indiponivel" << endl;
                cout << "Deseja fazer outra operacao?" << endl;
                cout << endl;
                menu_cout(Menu);

            }

            break;

        case 4: // Corta Imagem

            cout << "4 - Digite o nome do arquivo (nao eh necessario fornecer o tipo. Ex.: .ppm): \n";
            cin.getline (nome_arq, S_MAX);


            if (abre_img(nome_arq, img))
            {
                cout << "sua imagem tem altura:" << img.nl;
                cout << " e largura: " << img.nc << endl;
                cout << "entre com a linha do corte, coluna, altura e largura, respectivamente: \n" << endl;
                cin >> linhaCorte;
                cin >> colunaCorte;
                cin >> alturaCorte;
                cin >> larguraCorte;
                while((linhaCorte+alturaCorte) > img.nl || alturaCorte < 0 || (colunaCorte+larguraCorte) > img.nl || larguraCorte <0 || linhaCorte < 0 || colunaCorte < 0){
                            cout<< "Intervalo ínvalido" << endl;
                            cin >> linhaCorte;
                            cin >> colunaCorte;
                            cin >> alturaCorte;
                            cin >> larguraCorte;
                        }
                cout << endl;
                cout << endl;
                corta(img, img_out, linhaCorte, colunaCorte, alturaCorte, larguraCorte);
                cout << "entre com o nome do novo arquivo" << endl;
                cin.ignore();
                cin.getline(novo_nome, S_MAX);
                salva_img(novo_nome, img_out);
                reseta(img);
                reseta(img_out);
                cout << endl;
                cout << "Imagem pronta." << endl;
                cout << endl;
                cout << "Deseja continuar?" << endl;
                cout << endl;
                menu_cout(Menu);

            }
            else
            {
                cout << "\nEste arquivo esta indiponivel" << endl;
                cout << "Deseja fazer outra operacao?" << endl;
                cout << endl;
                menu_cout(Menu);

            }

            break;

        case 5:

            cout << "5 - Digite o nome do arquivo (nao eh necessario fornecer o tipo. Ex.: .ppm): \n";
            cin.getline (nome_arq, S_MAX);

            if (abre_img(nome_arq, img))
            {
                equaliza_histograma(img, img_out);
                cout << endl;
                cout << "entre com o nome do novo arquivo" << endl;
                cin.getline(novo_nome, S_MAX);
                salva_img(novo_nome, img_out);
                cout << endl;
                cout << "Imagem pronta." << endl;
                cout << endl;
                cout << "Deseja continuar?" << endl;
                cout << endl;
                menu_cout(Menu);

            }
            else
            {
                cout << "\nEste arquivo esta indiponivel" << endl;
                cout << "Deseja fazer outra operacao?" << endl;
                cout << endl;
                menu_cout(Menu);

            }
            break;

        }
    while(Menu <1 || Menu >6){
        menu_cout(Menu);
    }
    }
    cout << "Obrigado por utilizar nosso programa" << endl;





    return 0;

}



