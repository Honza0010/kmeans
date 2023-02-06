#include "Kmeans.h"

#include <cmath>

Bod::Bod(const std::vector<double>& souradnice, int dimenze)
{
    this->dimenze = dimenze;

    //this->souradnice = new double[this->dimenze];

    for (int i = 0; i < this->dimenze; i++)
    {
        this->souradnice.push_back(souradnice[i]);
    }
}

Bod::~Bod()
{
    
}

double Bod::operator[](int i) const
{
    if (i >= 0 && i < dimenze)
    {
        return this->souradnice[i];
    }
    else
    {
        throw MyException("Mimo rozsah souradnic");
    }
}

int Bod::vratDimenzi() const
{
    return this->dimenze;
}

Centroid::Centroid(const std::vector<double>& souradnice, int dimenze)
{
    this->dimenze = dimenze;

    //this->souradnice = new double[this->dimenze];

    for (int i = 0; i < this->dimenze; i++)
    {
        this->souradnice.push_back(souradnice[i]);
        pomocnesouradnice.push_back(0.0);
    }

    pocetBodu = 0;
}

Centroid::~Centroid()
{
    //delete[] souradnice;
}

double Centroid::operator[](int i) const
{
    if (i >= 0 && i < dimenze)
    {
        return this->souradnice[i];
    }
    else
    {
        throw MyException("Mimo rozsah souradnic");
    }
}

int Centroid::vratDimenzi() const
{
    return this->dimenze;
}

void Centroid::vycisti()
{
    for (int i = 0; i < pomocnesouradnice.size(); i++)
    {
        pomocnesouradnice[i] = 0.0;
    }
    body.clear();
    pocetBodu = 0;
}

void Centroid::priradBod(int i, const Bod& a)
{
    this->body.push_back(i);
    pocetBodu++;        //Pocet bodu v clusteru se zvetsi

    for (int i = 0; i < dimenze; i++)
    {
        this->pomocnesouradnice[i] += a[i];     //Na i-tou souradnici vlozime soucet i-tych souradnic vsech bodu v danem clusteru
    }
}

bool Centroid::noveSouradnice()
{
    if (pocetBodu == 0)
    {
        return false;
    }
    bool zmena = false;

    for (int i = 0; i < pomocnesouradnice.size(); i++)
    {
        pomocnesouradnice[i] /= pocetBodu;
        if (pomocnesouradnice[i] != souradnice[i])      //Pokud se alespon jedna nova souradnice lisi od jedne stare, tak se centroid hybe
        {
            zmena = true;
            souradnice[i] = pomocnesouradnice[i];
        }
    }

    return zmena;
    
}

int Centroid::vratPrirazenyBod(int i) const
{
    if(i >= 0 && i < body.size())
    {
        return this->body[i];
    }
    else
    {
        throw MyException("Mimo rozsah prirazenych bodu");
    }

}

int Centroid::vratPocetBodu() const
{
    return this->pocetBodu;
}

Kmeans::Kmeans(std::vector<Centroid> centroidy, std::vector<Bod> body, int dimenze)
{
    this->dimenze = dimenze;
    this->pocetBodu = body.size();
    this->pocetCentroidu = centroidy.size();

    for (int i = 0; i < pocetCentroidu; i++)
    {
        this->centroidy.push_back(centroidy[i]);
    }

    for (int i = 0; i < pocetBodu; i++)
    {
        this->body.push_back(body[i]);
    }

}

int Kmeans::vratDimenzi() const
{
    return this->dimenze;
}

int Kmeans::vratPocetCentroidu() const
{
    return this->pocetCentroidu;
}

const Centroid& Kmeans::operator[](int i) const
{
    if (i < this->pocetCentroidu && i >= 0)
    {
        return centroidy[i];
    }
    else
    {
        throw MyException("Out of range");
    }
}


double Kmeans::vzdalenost(const Bod& a, const Centroid& b)
{
    double vysledek = 0;
    for (int i = 0; i < a.vratDimenzi(); i++)
    {
        vysledek += (a[i] - b[i]) * (a[i] - b[i]);      //Eukleidovska metrika
    }
    vysledek = sqrt(vysledek);
    return vysledek;
}

int Kmeans::najdiNejblizsiCentroid(const Bod& a)        //Vrati index centroidu ve vectoru centroidy
{
    try
    {
        int centroid = 0;
        Centroid pom = centroidy[0];
        for (int j = 1; j < centroidy.size(); j++)
        {
            if (vzdalenost(a, pom) > vzdalenost(a, centroidy[j]))       //Porovnava vzdalenosti daneho bodu od vsech centroidu
            {
                pom = centroidy[j];
                centroid = j;
            }
        }

        return centroid;
    }
    catch(MyException ex)
    {
        throw MyException("Nepovedlo se najit");
    }
}

void Kmeans::priradCentroidumBody()
{
    for (int i = 0; i < body.size(); i++)
    {
      /*  int centroid = 0;
        Centroid pom = centroidy[0];
        for (int j = 0; j < centroidy.size(); j++)
        {
            if (vzdalenost(body[i], pom) > vzdalenost(body[i], centroidy[j]))
            {
                pom = centroidy[j];
                centroid = j;
            }
        }*/
        try
        {
            int centroid = najdiNejblizsiCentroid(body[i]);
            centroidy[centroid].priradBod(i, body[i]);
        } catch (MyException ex)
        {
            throw MyException("Nepovedlo se priradit body");
        }

    }
}

bool Kmeans::vypoctiTeziste()
{
    try
    {
        bool centroidySeHybou = false;

        for (int i = 0; i < centroidy.size(); i++)
        {
            if (centroidy[i].noveSouradnice())
            {
                centroidySeHybou = true;
            }
        }
        return centroidySeHybou;
    }
    catch(MyException ex)
    {
        throw MyException("Nepovedlo se vypocitat teziste");
    }
}

void Kmeans::vyres()
{
    try
    {
        priradCentroidumBody();
        while (vypoctiTeziste())
        {
            for (int i = 0; i < centroidy.size(); i++)
            {
                centroidy[i].vycisti();
            }
            priradCentroidumBody();
        }
    } catch (MyException ex)
    {
        throw MyException("Nepovedlo se vyresit kmeans");
    }

}
