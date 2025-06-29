/** main function definition for client side of Treasure Hunt game **/

#include "Client.hpp"
#include <cstdio>
#include <unistd.h>
#include <termios.h>

using namespace TreasureHunt;

bool IsNotMovementKey(char key) {
  return (key != 'w' && key != 'a' && key != 's' && key != 'd');
}

int main(int argc, char *argv[]) {
  struct termios newConfT;
  struct termios oldConfT;
  TreasureHunt::Client client;
  char key;
  int ret;

  tcgetattr(STDIN_FILENO, &oldConfT);
  newConfT = oldConfT;
  newConfT.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newConfT);

  printf("\nPress any key to start game...");
  getchar();

  do {
    client.PrintEmptySpace();
    client.PrintGrid();
    do {
      key = getchar();
    } while (IsNotMovementKey(key));

    switch(key) {
      case 'a':
        ret = client.InformServerMovement(CustomProtocol::MOVE_LEFT);
        if (ret == INVALID_MOVE) continue;
        client.Move(CustomProtocol::MOVE_LEFT);
        break;
      case 'w':
        ret = client.InformServerMovement(CustomProtocol::MOVE_UP);
        if (ret == INVALID_MOVE) continue;
        client.Move(CustomProtocol::MOVE_UP);
        break;
      case 's':
        ret = client.InformServerMovement(CustomProtocol::MOVE_DOWN);
        if (ret == INVALID_MOVE) continue;
        client.Move(CustomProtocol::MOVE_DOWN);
        break;
      case 'd':
        ret = client.InformServerMovement(CustomProtocol::MOVE_RIGHT);
        if (ret == INVALID_MOVE) continue;
        client.Move(CustomProtocol::MOVE_RIGHT);
        break;
    }

    if (ret == TREASURE_FOUND) {
      printf("+1 treasure found!\n");
      client.GetServerTreasure();
      client.ShowTreasure();
    }
  } while (!client.GameEnded());
  client.PrintEmptySpace();
  client.PrintGrid();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldConfT);

  return 0;
}