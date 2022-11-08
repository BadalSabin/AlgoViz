#pragma once
#include "Graph.h"
#include <queue>
#include <vector>
#include <QPlainTextEdit>
#include <QLabel>
#include "../animation/PauseAnim.h"
#include <limits>
#include <QTextCodec>
#include <qtextcodec>

class DijkstraSPF
{
private:
	Graph* dspfGraph;
	QGraphicsScene* graphicsScene;
	QPlainTextEdit* dspfConsole;
public:
	DijkstraSPF();
	DijkstraSPF(QGraphicsScene* graphicsScene, QPlainTextEdit* dspfConsole);
	~DijkstraSPF();

	void run(int startNodeId);
	void displayGraph();

private:
	int minDistance(std::vector<int>* dist, std::vector<bool>* sptSet);
	void printSolution(std::vector<int>* dist, int sourceNode);
};

