#pragma once

#include <vector>
#include "Exception.h"

class Bod
{
	std::vector<double> souradnice;		//Souradnice bodu
	int dimenze;	//Dimenze

public:
	Bod(const std::vector<double>& souradnice, int dimenze);
	~Bod();
	double operator[](int i) const;		//Operator [], ktery vrati i-tou souradnici
	int vratDimenzi() const;

	friend class Kmeans;
};


class Centroid
{
    std::vector<double> souradnice;	//Souradnice centroidu
	int dimenze; //Dimenze

	std::vector<int> body;		//Indexy bodu prislusejicich danemu centroidu
	int pocetBodu;				//Pocet bodu ve shluku

	std::vector<double> pomocnesouradnice;  //Soucet i-tych souradnic vsech bodu, ktere se posleze vydeli poctem bodu a dostaneme teziste

public:
	Centroid(const std::vector<double>& souradnice, int dimenze);
	~Centroid();
	double operator[](int i) const;			//Operator [], ktery vrati i-tou souradnici
	int vratDimenzi() const;
	void vycisti();			//Vynuluje pomocne souradnice a vycisti vector body
	void priradBod(int i, const Bod& a);
	bool noveSouradnice();		//Vypocte nove souradnice a zkontroluje, zda se zmenily
    int vratPrirazenyBod(int i) const; //vrati i-ty bod z vektoru body
    int vratPocetBodu() const;

	friend class Kmeans;
};



class Kmeans
{
	int dimenze;

	std::vector<Centroid> centroidy;
	std::vector<Bod> body;

	int pocetCentroidu;
	int pocetBodu;


	double vzdalenost(const Bod& a, const Centroid& b);		//Pocita vzdalenost mezi bodem a centroidem
	int najdiNejblizsiCentroid(const Bod& a);				//Nalezne centroid, ktery je danemu bodu nejblize
	void priradCentroidumBody();	//K jednotlivým centroidùm pøiøadí body podle vzdálenosti bodu od centroidu
	bool vypoctiTeziste();
	//void vycisti();		//Metoda vola metodu vycisti pro kazdy centroid ve vektoru centroidy

public:
	Kmeans(std::vector<Centroid> centroidy, std::vector<Bod> body, int dimenze);

	int vratDimenzi() const;
	int vratPocetCentroidu() const;

	const Centroid& operator[](int i) const;

	void vyres();
};
