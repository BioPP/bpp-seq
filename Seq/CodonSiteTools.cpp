/*
 * File CodonTools.cpp
 * Author : Sylvain Glémin <glemin@univ-montp2.fr>
 * Last modification : ? October 2004
*/

#include "CodonSiteTools.h"
#include "StandardCodonAlphabet.h"
#include "CodonAlphabet.h"
#include "SiteTools.h"
#include "GeneticCode.h"
#include "StandardGeneticCode.h"
#include "DNA.h"

// From Utils:
#include <Utils/MapTools.h>

// From NumCalc:
#include <NumCalc/NumTools.h>
using namespace NumTools;
#include <NumCalc/VectorTools.h>
using namespace VectorStatTools;
using namespace VectorFunctions;

// From the STL:
#include <cmath>



// Method to know if a codon site contains gap(s) or not
bool CodonSiteTools::hasGap(const Site & site) {
	for (unsigned int i = 0 ; i < site.size() ; i++) {
                if (site[i] < 0) return true;
	}
	return false;
}


// Method to know if a codon site contains undefined character or not
bool CodonSiteTools::hasUnknown(const Site & site) {
	for(unsigned int i = 0; i < site.size(); i++) {
		if(site[i] >= (int)site.getAlphabet()->getNumberOfTypes()) return true;
	}
	return false;
}


// Method to know if a codon site contains stop codon
bool CodonSiteTools::hasStop(const Site & site, const CodonAlphabet & alpha) {
	for(unsigned int i = 0; i < site.size(); i++) {
        if(alpha.isStop(site[i])) return true;
	}
	return false;
}


// Method to know if a codon site is complete or not (doesn't contains stop, gap or undefined character)
bool CodonSiteTools::isComplete(const Site & site) {
	// Main loop : for all characters in site
	for(unsigned int i = 0; i < site.size(); i++) {
		if(site[i] < 0 || site[i] >= (int)site.getAlphabet() -> getSize()) return false;
	}
	return true;
}

// Method to know if a codon site is constant or not (contain one character type only)
bool CodonSiteTools::isConstant(const Site & site) throw (EmptySiteException) {
	// Empty site checking
	if(site.size() == 0) throw EmptySiteException("CodonSiteTools::isConstant: Incorrect specified site", &site);
	// For all site's characters
	int s = site[0];
	for (unsigned int i = 1; i < site.size(); i++) if (site[i] != s) return false;

	return true;
}




//Method to know if a polymorphic codon site is polymorphic at only one site
bool CodonSiteTools::isMonoSitePolymorphic(const Site & site, const NucleicAlphabet & na, const CodonAlphabet & ca) throw (Exception) {
	// Empty site checking
	if(site.size() == 0) throw EmptySiteException("CodonSiteTools::isMonoSitePolymorphic: Incorrect specified site", &site);
        //Alphabet checking
        if(site.getAlphabet()->getAlphabetType()==ca.getAlphabetType()){
	        // Global polymorphism checking
	        if (CodonSiteTools::isConstant(site)) return false;
	        //initialisation of the 3 sub-sites ot the codon
                vector<int> pos1,pos2,pos3;
	        for(unsigned int i = 0; i < site.size(); i++) {
		        pos1.push_back(ca.getFirstPosition(site[i]));
		        pos2.push_back(ca.getSecondPosition(site[i]));
		        pos3.push_back(ca.getThirdPosition(site[i]));
	        }
                Site s1(pos1,&na), s2(pos2,&na), s3(pos3,&na);
	        //polymorphism checking for each sub-sites
	        unsigned int nbpol=0;
	        if(!SiteTools::isConstant(s1)) nbpol++;
	        if(!SiteTools::isConstant(s2)) nbpol++;
	        if(!SiteTools::isConstant(s3)) nbpol++;
	        if(nbpol>1) return false;
	        return true;
        }
        else throw AlphabetMismatchException("CodonSiteTools::isMonoSitePolymorphic: alphabet is not CodonAlphabet", &ca, site.getAlphabet());
}

//Method to know if polymorphism at a codon site is synonymous
bool CodonSiteTools::isSynonymousPolymorphic(const Site & site, const NucleicAlphabet & na, const CodonAlphabet & ca) throw (Exception) {
        //Empty site checking
    if(site.size() == 0) throw EmptySiteException("CodonSiteTools::isSynonymousPolymorphic Incorrect specified site", &site);
    //Alphabet checking
    if(site.getAlphabet()->getAlphabetType()==ca.getAlphabetType()){
        // Global polymorphism checking
        if (CodonSiteTools::isConstant(site)) return false;
	// Synonymous polymorphism checking
        StandardGeneticCode sgc(&na);
        vector<int> prot;
        int first_aa = sgc.translate(site[0]);
	for(unsigned int i = 1; i < site.size(); i++) {
	        int aa = sgc.translate(site[i]);
	        if (aa != first_aa) return false;
        }
   	return true;
    }
    else throw AlphabetMismatchException("CodonSiteTools::isSynonymousPolymorphic: alphabet is not CodonAlphabet", &ca, site.getAlphabet());
}

//Method to know the number of difference between two codons
unsigned int CodonSiteTools::NumberOfDifferences(int i, int j, const CodonAlphabet & ca){
       	unsigned int nbdif = 0;
        if(ca.getFirstPosition(i)!=ca.getFirstPosition(j)) nbdif++;
       	if(ca.getSecondPosition(i)!=ca.getSecondPosition(j)) nbdif++;
        if(ca.getThirdPosition(i)!=ca.getThirdPosition(j)) nbdif++;
        return nbdif;
}


double CodonSiteTools::NumberOfSynonymousDifferences(int i, int j, const CodonAlphabet & ca, const GeneticCode & gc, bool minchange){
        //Alphabet checking
        vector<int> ci=ca.getPositions(i);
        vector<int> cj=ca.getPositions(j);
        switch(CodonSiteTools::NumberOfDifferences(i,j,ca)) {
                case 0 : return 0;
                case 1 : {
			 if(gc.areSynonymous(i,j)) return 1;
			 return 0;
		 }
		 case 2 : {
			 if(gc.areSynonymous(i,j)) return 2;
                         vector<double> path(2,0); // Vector of number of synonymous changes per path (2 here)
                         vector<double> weight(2,1); //Weight to exclude path through stop codon
			 if(ci[0]==cj[0]) {
				 int trans1 = ca.getCodon(ci[0],cj[1],ci[2]); // transitory codon between NcNiNi et NcNjNj: NcNjNi, Nc = identical site
				 int trans2 = ca.getCodon(ci[0],ci[1],cj[2]); // transitory codon between NcNiNi et NcNjNj: NcNiNj, Nc = identical site
                                 if(!ca.isStop(trans1)){
				        if(gc.areSynonymous(i,trans1)) path[0]++;
				        if(gc.areSynonymous(trans1,j)) path[0]++;
                                 }
                                 else weight[0]=0;
                                 if(!ca.isStop(trans2)){
                                        if(gc.areSynonymous(i,trans2)) path[1]++;
                                        if(gc.areSynonymous(trans2,j)) path[1]++;
                                 }
                                 else weight[1]=0;
			 }
			 if(ci[1]==cj[1]) {
				 int trans1 = ca.getCodon(cj[0],ci[1],ci[2]); // transitory codon between NiNcNi et NjNcNj: NjNcNi, Nc = identical site
				 int trans2 = ca.getCodon(ci[0],ci[1],cj[2]); // transitory codon between NiNcNi et NjNcNj: NiNcNj, Nc = identical site
                                 if(!ca.isStop(trans1)){
				        if(gc.areSynonymous(i,trans1)) path[0]++;
				        if(gc.areSynonymous(trans1,j)) path[0]++;
                                 }
                                 else weight[0]=0;
                                 if(!ca.isStop(trans2)){
				        if(gc.areSynonymous(i,trans2)) path[1]++;
				        if(gc.areSynonymous(trans2,j)) path[1]++;
                                 }
                                 else weight[1]=0;

			 }
			 if(ci[2]==cj[2]) {
				 int trans1 = ca.getCodon(cj[0],ci[1],ci[2]); // transitory codon between NiNiNc et NjNjNc: NjNiNc, Nc = identical site
				 int trans2 = ca.getCodon(ci[0],cj[1],ci[2]); // transitory codon between NiNiNc et NjNjNc: NiNjNc, Nc = identical site
                                 if(!ca.isStop(trans1)){
				        if(gc.areSynonymous(i,trans1)) path[0]++;
				        if(gc.areSynonymous(trans1,j)) path[0]++;
                                 }
                                 else weight[0]=0;
                                 if(!ca.isStop(trans2)){
                                        if(gc.areSynonymous(i,trans2)) path[1]++;
				        if(gc.areSynonymous(trans2,j)) path[1]++;
                                 }
                                 else weight[1]=0;
			 }
                         if(minchange) return max(path);
                         double nbdif =0;
                         for(unsigned int i=0; i<2; i++) nbdif += path[i]*weight[i];
			 return nbdif/sum(weight);
                 }
                 case 3 : {
		 	vector<double> path(6,0);
                        vector<double> weight(6,1);
		 	//First transitory codons
		 	int trans100 = ca.getCodon(cj[0],ci[1],ci[2]);
		 	int trans010 = ca.getCodon(ci[0],cj[1],ci[2]);
		 	int trans001 = ca.getCodon(ci[0],ci[1],cj[2]);
		 	//Second transitory codons
		 	int trans110 = ca.getCodon(cj[0],cj[1],ci[2]);
		 	int trans101 = ca.getCodon(cj[0],ci[1],cj[2]);
		 	int trans011 = ca.getCodon(ci[0],cj[1],cj[2]);
		 	//Paths
                        if(!ca.isStop(trans100)){
		 	        if (gc.areSynonymous(i,trans100)) {path[0]++; path[1]++;}
                                if(!ca.isStop(trans110)){
                                        if (gc.areSynonymous(trans100,trans110)) path[0]++;
                                        if (gc.areSynonymous(trans110,j)) path[0]++;
                                }
                                else weight[0]=0;
                                if(!ca.isStop(trans101)){
		 	                if (gc.areSynonymous(trans100,trans101)) path[1]++;
                                        if (gc.areSynonymous(trans101,j)) path[1]++;
                                }
                                else weight[1]=0;
                        }
                        else {weight[0]=0;weight[1]=0;}
                        if(!ca.isStop(trans010)){
                                if (gc.areSynonymous(i,trans010)) {path[2]++;path[3]++;}
                                if(!ca.isStop(trans110)){
                                        if (gc.areSynonymous(trans010,trans110)) path[2]++;
                                        if (gc.areSynonymous(trans110,j)) path[2]++;
                                }
                                else weight[2]=0;
                                if(!ca.isStop(trans011)){
		 	                if (gc.areSynonymous(trans010,trans011)) path[3]++;
                                        if (gc.areSynonymous(trans011,j)) path[3]++;
                                }
                                else weight[3]=0;
                        }
                        else {weight[2]=0; weight[3]=0;}
                        if(!ca.isStop(trans001)){
		 	        if (gc.areSynonymous(i,trans001)) {path[4]++;path[5]++;}
                                if(!ca.isStop(trans101)){
                                        if (gc.areSynonymous(trans001,trans101)) path[4]++;
                                        if (gc.areSynonymous(trans101,j)) path[4]++;
                                }
                                else weight[4]=0;
                                if(!ca.isStop(trans011)){
		 	                if (gc.areSynonymous(trans001,trans011)) path[5]++;
                                        if (gc.areSynonymous(trans011,j)) path[5]++;
                                }
                                else weight[5]=0;
                        }
                        else {weight[4]=0; weight[5]=0;}
                        if(minchange) return max(path);
                        double nbdif=0;
                        for(unsigned int i=0; i<6; i++) nbdif += path[i]*weight[i];
                        return nbdif/sum(weight);
		 }
         }
}


//Method to compute the synonymous pi per codon site
//Here, the following formula is used: pi = n/(n-1) Sum[Xi*Xj*Pij]
//where n is the number of sequence, Xi and Xj the frequencies of each codon type occuring at the site, Pij the number of synonymous difference between these codon
//Attention: pi is not normalized by the number of synonymous sites
double CodonSiteTools::piSynonymous(const Site & site, const CodonAlphabet & ca, const GeneticCode & gc, bool minchange) throw(Exception) {
        //Empty site checking
        if(site.size() == 0) throw EmptySiteException("CodonSiteTools::isSynonymousPolymorphic Incorrect specified site", &site);
        //Alphabet checking
        if(site.getAlphabet()->getAlphabetType()==ca.getAlphabetType()){
                //General polymorphism checking
                if (CodonSiteTools::isConstant(site)) return 0;
                //Computation
                map<int,double> freq = SiteTools::getFrequencies(site);
                double pi = 0;
                for(map<int,double>::iterator it1 = freq.begin(); it1 != freq.end(); it1++) {
		        for(map<int,double>::iterator it2 = freq.begin(); it2 != freq.end(); it2++) {
			        pi += (it1->second)*(it2->second)*(CodonSiteTools::NumberOfSynonymousDifferences(it1->first,it2->first,ca,gc,minchange));
		        }
	        }
                unsigned int n = site.size();
                return pi*n/(n-1);
        }
        else throw AlphabetMismatchException("CodonSiteTools::piSynonymous: alphabet is not CodonAlphabet", &ca, site.getAlphabet());

}

//Method to compute the non-synonymous pi per codon site
//Here, the following formula is used: pi = n/(n-1) Sum[Xi*Xj*Pij]
//where n is the number of sequence, Xi and Xj the frequencies of each codon type occuring at the site, Pij the number of synonymous difference between these codon
//Attention: pi is not normalized by the number of non-synonymous sites
double CodonSiteTools::piNonSynonymous(const Site & site, const NucleicAlphabet & na, const CodonAlphabet & ca, const GeneticCode & gc, bool minchange) throw(Exception) {
	//Empty site checking
        if(site.size() == 0) throw EmptySiteException("CodonSiteTools::isSynonymousPolymorphic Incorrect specified site", &site);
        //Alphabet checking
        if(site.getAlphabet()->getAlphabetType()==ca.getAlphabetType()){
                //General polymorphism checking
                if (CodonSiteTools::isConstant(site)) return 0;
                if (CodonSiteTools::isSynonymousPolymorphic(site,na,ca)) return 0;
                //Computation
                map<int,double> freq = SiteTools::getFrequencies(site);
                double pi = 0;
                for(map<int,double>::iterator it1 = freq.begin(); it1 != freq.end(); it1++) {
		        for(map<int,double>::iterator it2 = freq.begin(); it2 != freq.end(); it2++) {
			        unsigned int nbtot=CodonSiteTools::NumberOfDifferences(it1->first,it2->first,ca);
			        unsigned int nbsyn=CodonSiteTools::NumberOfSynonymousDifferences(it1->first,it2->first,ca, gc, minchange);
			        pi += (it1->second)*(it2->second)*(nbtot-nbsyn);
		        }
	        }
	        unsigned int n = site.size();
	        return pi*n/(n-1);
        }
        else throw AlphabetMismatchException("CodonSiteTools::piNonSynonymous: alphabet is not CodonAlphabet", &ca, site.getAlphabet());
}


//Method that gives the number of synonymous positions of a codon
//Transition/transversion ratio is taken into account. Default option ratio=1
double CodonSiteTools::NumberOfSynonymousPositions(int i, const CodonAlphabet & ca, const GeneticCode & gc, double ratio) {
	if(ca.getName(ca.intToChar(i))=="STOP") return 0;
        int acid=gc.translate(i);
	switch (gc.getSynonymous(acid).size()){
		case 1: return 0;
		case 2: return ratio/(ratio+2);
		case 3: {
                        if(i==14) return 2/(ratio+2);
			return (ratio+1)/(ratio+2);
		}
		case 4:	return 1;
		case 6: {
			if(i==24||i==26||i==28||i==30||i==51||i==53) return 1+ratio/(ratio+2);
			if(i==25||i==27||i==29||i==31||i==50||i==52) return 1;
			if(i==8||i==9||i==10||i==11||i==57||i==59) return 2*ratio/(ratio+2);
		}
	}
}

// Method that gives the mean number of synonymous position per codon site
//Transition/transversion ratio is taken into account. Default option ratio=1
double CodonSiteTools::MeanNumberOfSynonymousPositions(const Site & site, const CodonAlphabet & ca, const GeneticCode & gc, double ratio) throw(Exception) {
	//Empty site checking
	if(site.size() == 0) throw EmptySiteException("CodonSiteTools::MeanNumberOfSynonymousPositions Incorrect specified site", &site);
        //Alphabet checking
        if(site.getAlphabet()->getAlphabetType()==ca.getAlphabetType()){
	        //Computation
	        double NbSyn=0;
	        map<int,double> freq = SiteTools::getFrequencies(site);
			for(map<int,double>::iterator it = freq.begin(); it != freq.end(); it++) {
				NbSyn += (it->second)*NumberOfSynonymousPositions(it->first,ca,gc,ratio);
			}
        return NbSyn;
        }
        else throw AlphabetMismatchException("CodonSiteTools::MeanNumberOfSynonymousPositions: alphabet is not CodonAlphabet", &ca, site.getAlphabet());
}






