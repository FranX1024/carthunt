#include <stdio.h>
#include <math.h>
#include <vector>

class Point2d {
public:
  double x, y;
  Point2d(int x, int y) {
    this->x = x;
    this->y = y;
  }
};

char mapa[100][100];

// fill all matrix elements with 1 if they're covered by the polygon
void drawConvexPolygon(std::vector<Point2d> polygon) {
    
  // find top and bottom row polygon is contained in
  double miny = polygon[0].y;
  double maxy = polygon[0].y;
  for(int i = 0; i < polygon.size(); i++) {
    miny = std::min(miny, polygon[i].y);
    maxy = std::max(maxy, polygon[i].y);
  }
  int bottom_row = floor(maxy);
  int top_row = floor(miny);
  int rows = bottom_row - top_row + 1;
    
  // arrays to store leftmost and rightmost column for each row
  int* left_border = new int[rows];
  int* right_border = new int[rows];
    
  for(int i = 0; i < rows; i++) {
    left_border[i] = 1e9;
    right_border[i] = -1e9;
  }
    
  for(int i = 0; i < polygon.size(); i++) {
        
    // get the next point to form a line
    int j = i + 1;
    if(j == polygon.size()) j = 0;
        
    // rows through which the line spans
    double top_y = std::min(polygon[i].y, polygon[j].y);
    double bottom_y = std::max(polygon[i].y, polygon[j].y);
    int line_top = floor(top_y);
    int line_bottom = floor(bottom_y);
        
    // special case if the line is contained within a single row
    if(line_top == line_bottom) {
      left_border[line_top - top_row] = std::min(left_border[line_top - top_row], (int)floor(std::min(polygon[i].x, polygon[j].x)));
      right_border[line_top - top_row] = std::min(right_border[line_top - top_row], (int)floor(std::max(polygon[i].x, polygon[j].x)));
      continue;
    }
        
    double y2x = (polygon[i].x - polygon[j].x) / (polygon[i].y - polygon[j].y);
        
    for(int row = line_top; row <= line_bottom; row++) {
      double minx, maxx;
      minx = polygon[i].x + y2x * (std::max((double) row, top_y) - polygon[i].y);
      maxx = polygon[i].x + y2x * (std::min((double) row + 1, bottom_y) - polygon[i].y);
      if(minx > maxx) std::swap(minx, maxx);
            
      left_border[row - top_row] = std::min(left_border[row - top_row], (int)floor(minx));
      right_border[row - top_row] = std::max(right_border[row - top_row], (int)floor(maxx));
    }
  }
    
  // fill all cells under polygon
  for(int row = top_row; row <= bottom_row; row++) {
    for(int col = left_border[row - top_row]; col <= right_border[row - top_row]; col++) {
      mapa[row][col] = '#';
    }
  }

  for(int i = 0; i < polygon.size(); i++) {
    mapa[(int)polygon[i].y][(int)polygon[i].x] = 'x';
  }
  
  delete left_border;
  delete right_border;
}

int main() {

  for(int row = 0; row < 100; row++) {
    for(int col = 0; col < 100; col++) {
      mapa[row][col] = '.';
    }
  }

  double angl = 3.14159265 / 3;
    
  std::vector<Point2d> pts;
  for(int i = 0; i < 6; i++) {
    pts.push_back(Point2d(10 + 8 * sin(angl * i),
			  10 + 8 * cos(angl * i)));
  }
  drawConvexPolygon(std::vector<Point2d>{
      Point2d(5, 5),
      Point2d(13, 7),
      Point2d(19, 0)
      });
    
  for(int row = 0; row < 20; row++) {
    for(int col = 0; col < 20; col++) {
      printf("%c", mapa[row][col]);
    }
    printf("\n");
  }
  return 0;
}
