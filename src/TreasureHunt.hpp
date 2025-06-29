#ifndef TREASUREHUNT_HPP_
#define TREASUREHUNT_HPP_

namespace TreasureHunt {

const int TREASURE_FOUND = 1;
const int VALID_MOVE = 2;
const int INVALID_MOVE = 3;

const int TOTAL_TREASURES = 8;
const int GRID_SIZE = 8;
const int INI_X = 0;
const int INI_Y = 0;
const char TREASURES_DIR[] = "./objetos/";

enum FileType {
  TXT = 1,
  MP4,
  JPG
};

struct Position {
  int x;
  int y;
  inline void SetPosition(int newX, int newY) {
    this->x = newX;
    this->y = newY;
  }
  inline bool Equal(int x, int y) const {
    return (this->x == x && this->y == y);
  }
  inline void MoveUp() {
    this->x++;
  }
  inline void MoveDown() {
    this->x--;
  }
  inline void MoveRight() {
    this->y++;
  }
  inline void MoveLeft() {
    this->y--;
  }
};

}

#endif