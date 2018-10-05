#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include "ConfigFile.tpp" // Fichier .tpp car inclut un template

using namespace std;

class Engine
{

private:
  double t, tfin; // Temps courant et temps final
  unsigned int nsteps; // Nombre d'iterations
  double B0, Kappa; // Intensite et gradient du champ magnetique
  double E;   // Intensite du champ electrique
  double m, q;  // Masse et charge de la particule
  double x0, y0, vx0, vy0;  // Position et vitesse initiales de la particle
  unsigned int sampling; // Nombre d'iterations entre chaque ecriture des diagnostics
  unsigned int last; // Nombre d'iterations depuis la derniere ecriture des diagnostics
  ofstream *outputFile; // Pointeur vers le fichier de sortie
  
  // Variables personnelles
  double vxold, vyold; // Stockage d'anciennes valeurs de vx et vy
  double k2; // Variable k2 pour la methode de Runge-Kutta
  double lambda(q/m);
  double omega(lambda*B0);

  // Ecriture des diagnostics
  void printOut(bool force)
  {
    // Ecriture tous les [sampling] pas de temps, sauf si force est vrai
    if((!force && last>=sampling) || (force && last!=1))
    {
      double energy = 0.; // TODO: Completer l'expression de l'energie
      double mu = 0.; // TODO: Completer l'expression du moment magnetique
      *outputFile << t << " " << x << " " << y << " " << vx << " " << vy << " " << energy << " " << mu << endl;
      last = 1;
    }
    else
    {
      last++;
    }
  }

  // Iteration temporelle, a definir au niveau des classes filles
  virtual void step()=0;

  // Champ magnetique variable
  double B(double const& x) const
  {
    return B0 * (1. + Kappa*x);
  }

protected:
  double dt; // Pas de temps
  double x, y, vx, vy;  // Position et vitesse de la particle

public:

  // Constructeur
  Engine(ConfigFile configFile)
  {
    // Stockage des parametres de simulation dans les attributs de la classe
    tfin     = configFile.get<double>("tfin");
    nsteps   = configFile.get<unsigned int>("nsteps");
    dt       = tfin / nsteps;
    m        = configFile.get<double>("m");
    q        = configFile.get<double>("q");
    B0       = configFile.get<double>("B0");
    Kappa    = configFile.get<double>("Kappa");
    E        = configFile.get<double>("E");
    x0       = configFile.get<double>("x0");
    y0       = configFile.get<double>("y0");
    vx0      = configFile.get<double>("vx0");
    vy0      = configFile.get<double>("vy0");
    sampling = configFile.get<unsigned int>("sampling");

    // Ouverture du fichier de sortie
    outputFile = new ofstream(configFile.get<string>("output").c_str());
    outputFile->precision(15); // Les nombres seront ecrits avec 15 decimales
  };

  // Destructeur virtuel
  virtual ~Engine()
  {
    outputFile->close();
    delete outputFile;
  };

  // Simulation complete
  void run()
  {
    t = 0.;
    x = x0;
    y = y0;
    vx = vx0;
    vy = vy0;
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

class EngineEuler: public Engine
{
public:
  EngineEuler(ConfigFile configFile): Engine(configFile) {}

  void step()
  {
    // TODO: Mettre a jour x, y, vx, vy avec le schema d'Euler
    vxold = vx;
    vyold = vy;
    vx = vxold + q/m*B0*vyold*dt;
    vy = vyold + q/m*(E-B0*vxold)*dt;
    x = x + vxold*dt;
    y = y + vyold*dt;
  }
};

class EngineEulerCromer: public Engine
{
public:
  EngineEulerCromer(ConfigFile configFile): Engine(configFile) {}

  void step()
  {
    // TODO: Mettre a jour x, y, vx, vy avec le schema d'Euler-Cromer
    vx = vx + q/m*B0*vy*dt;
    vy = vy + q/m*(E-B0*vx)*dt;
    x = x + vx*dt;
    y = y + vy*dt;
  }
};

class EngineRungeKutta2: public Engine
{
public:
  EngineRungeKutta2(ConfigFile configFile): Engine(configFile) {}

  void step()
  {
    // TODO: Mettre a jour x, y, vx, vy avec le schema de Runge-Kutta d'ordre 2
    vxold = vx;
    vyold = vy;
    k2 = omega*(vyold + 0.5*dt*lambda*(E - B0*vxold))*dt;
    vx = vxold + k2;
    k2 = lambda*(E - B0*(vxold + 0.5*dt*omega*vyold))*dt;
    vy = vyold + k2;
    k2 = (vxold + 0.5*dt*omega*vyold)*dt;
    x = x + k2;
    k2 = (vyold + 0.5*dt*lambda*(E - B0*vxold))*dt;
    y = y + k2;
  }
};


int main(int argc, char* argv[])
{
  string inputPath("configuration.in"); // Fichier d'input par defaut
  if(argc>1) // Fichier d'input specifie par l'utilisateur ("./Exercice2 config_perso.in")
    inputPath = argv[1];

  ConfigFile configFile(inputPath); // Les parametres sont lus et stockes dans une "map" de strings.

  for(int i(2); i<argc; ++i) // Input complementaires ("./Exercice2 config_perso.in input_scan=[valeur]")
    configFile.process(argv[i]);

  // Schema numerique ("Euler"/"E", "EulerCromer"/"EC" ou "RungeKutta2"/"RK2")
  string schema(configFile.get<string>("schema"));

  Engine* engine;
  if(schema == "Euler" || schema == "E")
  {
    engine = new EngineEuler(configFile);
  }
  else if(schema == "EulerCromer" || schema == "EC")
  {
    engine = new EngineEulerCromer(configFile);
  }
  else if(schema == "RungeKutta2" || schema == "RK2")
  {
    engine = new EngineRungeKutta2(configFile);
  }
  else
  {
    cerr << "Schema inconnu" << endl;
    return -1;
  }

  engine->run();

  delete engine;
  cout << "Fin de la simulation." << endl;
  return 0;
}
