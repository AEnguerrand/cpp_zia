/*
** main.cpp for CPP_ZIA in /home/enguerrand/delivery/CPP_ZIA/main.cpp
**
** Made by Enguerrand Allamel
** Login   <enguerrand.allamel@epitech.eu>
**
** Started on  Wed Oct 04 17:01:02 2017 Enguerrand Allamel
** Last update Wed Oct 04 17:01:02 2017 Enguerrand Allamel
*/

#include "Console/Console.hh"
#include "zia.hh"

int main(int, char **) {
  nz::zia zia;
  nz::Console console(zia);

  zia.start();
  console.run();
  return (0);
}
