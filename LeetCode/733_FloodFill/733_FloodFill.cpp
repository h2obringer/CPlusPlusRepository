#include <iostream>
#include <vector>
#include <queue>
using namespace std;

/* https://leetcode.com/explore/learn/card/queue-stack/239/conclusion/1393/
https://leetcode.com/problems/flood-fill

PROBLEM: An image is represented by an m x n integer grid image where image[i][j] represents the pixel value of the image.
You are also given three integers sr, sc, and color. You should perform a flood fill on the image starting from the pixel image[sr][sc].
To perform a flood fill, consider the starting pixel, plus any pixels connected 4-directionally to the starting pixel of the same color as the starting pixel, plus any pixels connected 4-directionally to those pixels (also with the same color), and so on. Replace the color of all of the aforementioned pixels with color.
Return the modified image after performing the flood fill.

Example 1:
Input: image = [[1,1,1],[1,1,0],[1,0,1]], sr = 1, sc = 1, color = 2
Output: [[2,2,2],[2,2,0],[2,0,1]]
Explanation: From the center of the image with position (sr, sc) = (1, 1) (i.e., the red pixel), all pixels connected by a path of the same color as the starting pixel (i.e., the blue pixels) are colored with the new color.
Note the bottom corner is not colored 2, because it is not 4-directionally connected to the starting pixel.

Example 2:

Input: image = [[0,0,0],[0,0,0]], sr = 0, sc = 0, color = 0
Output: [[0,0,0],[0,0,0]]
Explanation: The starting pixel is already colored 0, so no changes are made to the image.

CONSTRAINTS:
    m == image.length
    n == image[i].length
    1 <= m, n <= 50
    0 <= image[i][j], color < 216
    0 <= sr < m
    0 <= sc < n
*/

class Solution {
private:
	int rowCount = 0;
	int colCount = 0;
public:
	vector<vector<int>> floodFill(vector<vector<int>>& image, int sr, int sc, int newColor) {
		rowCount = image.size();
		if(rowCount == 0) return image;
		colCount = image[0].size();

		int oldColor = image[sr][sc];
		if (oldColor == newColor) return image; //if the starting block is already the color we want we are done

		queue<pair<int, int>> q;
		q.push(pair<int, int> (sr, sc));
		image[sr][sc] = newColor; //mark it as visited...

		int next[4][2] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };

		while (!q.empty()) {
			int row = q.front().first;
			int col = q.front().second;
			q.pop();

			for (int i = 0; i < 4; i++) {
				int neighbor_row = row + next[i][0];
				int neighbor_col = col + next[i][1];
				if (neighbor_row >= 0 && neighbor_row < rowCount && neighbor_col >= 0 && neighbor_col < colCount && image[neighbor_row][neighbor_col] == oldColor) {
					image[neighbor_row][neighbor_col] = newColor; //mark it as visited

					q.push(pair<int, int>(neighbor_row, neighbor_col));
				}
			}
		}

		return image;
	}
};

//DFS Solution
class Solution2 {
public:
	vector<vector<int>> floodFill(vector<vector<int>>& image, int sr, int sc, int newColor) {
		int oldColor = image[sr][sc];
		if (oldColor == newColor) return image; //no work to be done...

		floodFill(image, sr, sc, oldColor, newColor);

		return image;
	}

	void floodFill(vector<vector<int>>& image, int sr, int sc, int& oldcolor, int& newcolor) {
		if (image[sr][sc] == oldcolor) {
			image[sr][sc] = newcolor;
			if (sc > 0) floodFill(image, sr, sc - 1, oldcolor, newcolor);
			if (sc + 1 < image[0].size()) floodFill(image, sr, sc + 1, oldcolor, newcolor);
			if (sr > 0) floodFill(image, sr - 1, sc, oldcolor, newcolor);
			if (sr + 1 < image.size()) floodFill(image, sr + 1, sc, oldcolor, newcolor);
		}
	}
};

//helper
void print(vector<vector<int>> image) {
	for (int row = 0; row < image.size(); row++) {
		for (int col = 0; col < image[0].size(); col++) {
			cout << image[row][col] << " ";
		}
		cout << endl;
	}
}

int main()
{
	Solution s;
	vector<vector<int>> image1 = { {0, 0, 0},{0, 0, 0} };
	
	print(s.floodFill(image1, 1, 0, 2));
}