#include <iostream>
#include <cmath>
#include <complex>
#include <iomanip>

using namespace std;

// ABSTRACT BASE CLASS
class ElectricalSystem {
public:
    virtual void calculate() = 0;
};

// PHASOR CLASS
class Phasor {

protected:
    double magnitude;
    double angle;

public:

    Phasor(double mag = 0, double ang = 0) {
        magnitude = mag;
        angle = ang;
    }

    // Convert phasor to complex number
    complex<double> toComplex() {

        double radians =
            angle * M_PI / 180.0;

        return polar(magnitude, radians);
    }
};

// POWER ANALYZER CLASS
class PowerAnalyzer {

protected:
    complex<double> voltage;
    complex<double> current;

public:

    PowerAnalyzer(
        complex<double> v,
        complex<double> i)
    {
        voltage = v;
        current = i;
    }

    // Complex Power
    complex<double> calculateComplexPower() {

        return voltage * conj(current);
    }

    // Real Power
    double getRealPower() {

        return calculateComplexPower().real() / 1000;
    }

    // Reactive Power
    double getReactivePower() {

        return calculateComplexPower().imag() / 1000;
    }

    // Apparent Power
    double getApparentPower() {

        return abs(calculateComplexPower()) / 1000;
    }

    // Power Factor
    double getPowerFactor() {

        return getRealPower() /
               getApparentPower();
    }
};

// POWER FACTOR CORRECTION CLASS
class PowerFactorCorrection :
    public ElectricalSystem,
    public PowerAnalyzer {

private:
    double targetPF;
    double capacitorKVAR;

public:

    PowerFactorCorrection(
        complex<double> v,
        complex<double> i,
        double pf)

        : PowerAnalyzer(v, i)
    {
        targetPF = pf;
        capacitorKVAR = 0;
    }

    // POLYMORPHISM
    void calculate() override {

        double P = getRealPower();

        double initialPF =
            getPowerFactor();

        cout << fixed
             << setprecision(3);

        cout << "\n=====================================\n";
        cout << "             RESULTS                 \n";
        cout << "=====================================\n";

        complex<double> S =
            calculateComplexPower() / 1000.0;

        cout << "Complex Power      : "
             << S.real()
             << " + j"
             << S.imag()
             << " kVA\n";

        cout << "Real Power         : "
             << getRealPower()
             << " kW\n";

        cout << "Reactive Power     : "
             << getReactivePower()
             << " kVAR\n";

        cout << "Apparent Power     : "
             << getApparentPower()
             << " kVA\n";

        cout << "Initial PF         : "
             << initialPF
             << endl;

        // CHECK IF CORRECTION IS NEEDED
        if(initialPF >= targetPF)
        {
            cout << "\nPower factor correction "
                 << "not required.\n";

            capacitorKVAR = 0;
        }
        else
        {
            double phi1 =
                acos(initialPF);

            double phi2 =
                acos(targetPF);

            capacitorKVAR =
                P *
                (tan(phi1) - tan(phi2));

            cout << "Required Capacitor : "
                 << capacitorKVAR
                 << " kVAR\n";
        }

        cout << "=====================================\n";
    }
};

int main() {

    double Vm, Va;
    double Im, Ia;
    double targetPF;

    cout << "=========================================\n";
    cout << " PHASOR POWER FACTOR CORRECTION SYSTEM\n";
    cout << "=========================================\n";

    // VOLTAGE INPUT
    cout << "\nEnter Voltage Magnitude (V): ";
    cin >> Vm;

    cout << "Enter Voltage Angle (degrees): ";
    cin >> Va;

    // CURRENT INPUT
    cout << "\nEnter Current Magnitude (A): ";
    cin >> Im;

    cout << "Enter Current Angle (degrees): ";
    cin >> Ia;

    // TARGET PF
    cout << "\nEnter Desired Power Factor: ";
    cin >> targetPF;

    // VALIDATION
    if(targetPF <= 0 || targetPF > 1)
    {
        cout << "\nInvalid Power Factor!\n";
        return 1;
    }

    // CREATE PHASORS
    Phasor voltagePhasor(Vm, Va);
    Phasor currentPhasor(Im, Ia);

    // CONVERT TO COMPLEX FORM
    complex<double> V =
        voltagePhasor.toComplex();

    complex<double> I =
        currentPhasor.toComplex();

    // CREATE SYSTEM OBJECT
    PowerFactorCorrection system(
        V,
        I,
        targetPF
    );

    // RUN CALCULATIONS
    system.calculate();

    return 0;
}
