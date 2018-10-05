#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include "ConfigFile.tpp" // Fichier .tpp car inclut un template

using namespace std;

class Engine
{

private:
  double t, tfin, dt; // Temps courant, temps final et pas de temps
  unsigned int nsteps; // Nombre d'iterations
  double G; // Constante gravitationnelle
  double mT, mL, zT, zL; // Masses et positions de la Terre et de la Lune
  double m, R; // Masse et rayon du projectile
  double a; //Acceleration
  double z0, v0; // Position et vitesse initiales du projectile
  double z, v; // Position et vitesse du projectile au cours du temps
  double rho0, lambda; // Parametres de la densite de l'air
  double Cx; // Coefficient de trainee aerodynamique
  unsigned int sampling; // Nombre d'iterations entre chaque ecriture des diagnostics
  unsigned int last; // Nombre d'iterations depuis la derniere ecriture des diagnostics
  ofstream *outputFile; // Pointeur vers le fichier de sortie

  // Ecriture des diagnostics
  void printOut(bool force)
  {
    // Ecriture tous les [sampling] pas de temps, sauf si force est vrai
    if((!force && last>=sampling) || (force && last!=1))
    {
      *outputFile << t << " " << z << " " << v << endl;
      last = 1;
    }
    else
    {
      last++;
    }
  }

  // Iteration temporelle
  void step()
  {
    // TODO: Mettre a jour z et v avec le schema d'Euler
    a = (z/abs(z))*G*(mL/pow(z-zL,2) - mT/(z*z)) - (v/abs(v))*rho0*exp(-(z-z0)/lambda)*M_PI*R*R*Cx*v*v/(2*m);
    z = z + v*dt;
    v = v + a*dt;
  }
/* - (rho0 * M_PI * R * R * Cx * v*v) / 2 */

public:

  // Constructeur
  Engine(int argc, char* argv[]) {

    string inputPath("configuration.in"); // Fichier d'input par defaut
    if(argc>1) // Fichier d'input specifie par l'utilisateur ("./Exercice1 config_perso.in")
      inputPath = argv[1];

    ConfigFile configFile(inputPath); // Les parametres sont lus et stockes dans une "map" de strings.

    for(int i(2); i<argc; ++i) // Input complementaires ("./Exercice1 config_perso.in input_scan=[valeur]")
      configFile.process(argv[i]);

    // Stockage des parametres de simulation dans les attributs de la classe
    tfin     = configFile.get<double>("tfin");
    nsteps   = configFile.get<unsigned int>("nsteps");
    dt       = tfin / nsteps;
    G        = configFile.get<double>("G");
    mT       = configFile.get<double>("mT");
    mL       = configFile.get<double>("mL");
    zT       = configFile.get<double>("zT");
    zL       = configFile.get<double>("zL");
    m        = configFile.get<double>("m");
    R        = configFile.get<double>("R");
    z0       = configFile.get<double>("z0");
    v0       = configFile.get<double>("v0");
    rho0     = configFile.get<double>("rho0");
    lambda   = configFile.get<double>("lambda");
    Cx       = configFile.get<double>("Cx");
    sampling = configFile.get<unsigned int>("sampling");

    // Ouverture du fichier de sortie
    outputFile = new ofstream(configFile.get<string>("output").c_str());
    outputFile->precision(15); // Les nombres seront ecrits avec 15 decimales
  };

  // Destructeur
  ~Engine()
  {
    outputFile->close();
    delete outputFile;
  };

  // Simulation complete
  void run()
  {
    t = 0;
    z = z0;
    v = v0;
    last = 0;
    printOut(true);
    for(unsigned int i(0); i<nsteps; ++i)
    {
      step();
      t += dt;
      printOut(false);
    }
    printOut(true);
  };

};


int main(int argc, char* argv[])
{
  Engine engine(argc, argv);
  engine.run();
  cout << "Fin de la simulation." << endl;
  return 0;
}
