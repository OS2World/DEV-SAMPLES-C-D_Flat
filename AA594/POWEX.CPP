// powex.cpp -- Implementation of Power function
// requires Borland C++ 4.0 or ANSI C++ compiler with exceptions 
// Copyright (c) 1994 by Tom Swan. All rights reserved

#include <iostream.h>
#include <math.h>

class Error;

double Pow(double b, double e);
double Power(double b, double e) throw(Error);

class Error {
  double b;    // Base
  double e;    // Exponent
public:
  Error()
    { cout << "Implementation error!" << endl; }
  Error(double bb, double ee)
    : b(bb), e(ee) { }
  void Report();
};

int main()
{
  cout << "Power Demonstration\n\n";
  cout << "This program displays the result of raising\n";
  cout << "a value (base) to a power (exponent). To\n";
  cout << "force an exception, enter a negative base\n";
  cout << "and a fractional exponent (e.g. -4 and 1.5)\n";
  cout << "Or, enter a zero base and an exponent less than\n";
  cout << "zero.\n\n";
  try {
    double base, exponent, result;
    cout << "base? ";
    cin >> base;
    cout << "exponent? ";
    cin >> exponent;
    result = Power(base, exponent);
    cout << "result == " << result << endl;
  }
  catch (Error& e) {
    e.Report();
    return -1;
  }
  return 0;
}

// Subfunction to Power
double Pow(double b, double e)
{
  return exp(e * log(b));
}

// Return b raised to the e power
double Power(double b, double e) throw(Error)
{
  if (b > 0.0) return Pow(b, e);
  if (b < 0.0) {
    double ipart;
    double fpart = modf(e, &ipart);
    if (fpart == 0) {
      if (fmod(ipart, 2) != 0)  // i.e. ipart is odd
        return -Pow(-b, e);
      else
        return Pow(-b, e);
    } else
      throw Error(b, e);
  } else {
    if (e == 0.0) return 1.0;
    if (e < 1.0) throw Error(b, e);
    return 0.0;
  }
//  throw Error();  // Unreachable code warning expected
}

// Display values that caused an exception
void
Error::Report()
{
  cout << "Domain error:"
    << " base:" << b
    << " exponent:" << e
    << endl
    << "Press Enter to continue...";
  char c;
  char buffer[80];
  if (cin.peek() == '\n') cin.get(c);
  cin.getline(buffer, sizeof(buffer) - 1);
}

