#include "Graph.h"

Graph::Graph()
	:graphicsScene(nullptr), nodesInGraph(nullptr), nodeNeighbours(nullptr)
{
}

Graph::Graph(QGraphicsScene* graphicsScene, bool weighted)
	:graphicsScene(graphicsScene), weighted(weighted)
{
	this->nodesInGraph = new std::vector<Node*>;
	this->linesInGraph = new std::vector<Line*>;
	this->nodeNeighbours = new std::map<int, std::vector<int>*>;
	this->nodePairByLevel = new std::map<int, std::pair<Node*, Node*>*>;
	if (weighted) {
		createWeightedStaticGraph();
	}
	else {
		createStaticGraph();
	}
}

Graph::~Graph()
{
}

void Graph::displayGraphNodes()
{
	for (std::vector<Node*>::iterator it = this->nodesInGraph->begin(); it != this->nodesInGraph->end(); it++)
	{
		this->graphicsScene->addItem(*it);
	}
	
	for (std::vector<Line*>::iterator it = this->linesInGraph->begin(); it != this->linesInGraph->end(); it++)
	{
		this->graphicsScene->addItem(*it);
	}
}

std::vector<Node*>* Graph::getNodesInGraph()
{
	return this->nodesInGraph;
}

std::vector<Line*>* Graph::getLinesInGraph()
{
	return this->linesInGraph;
}

std::map<int, std::pair<Node*, Node*>*>* Graph::getNodePairByLevel()
{
	return this->nodePairByLevel;
}

std::map<int, std::vector<int>*>* Graph::getNodeNeighbours()
{
	return this->nodeNeighbours;
}

void Graph::createGraphNode()
{
	Node* graphNode = nullptr;
	for (int i = 0; i < totalNodesInGraph; i++)
	{
		QString nodeVal = QString::number(i);
		graphNode = new Node(nodeVal, Qt::lightGray);
		this->nodesInGraph->emplace_back(graphNode);
	}
}

void Graph::createConnections()
{
	Node* currentNode = nullptr;
	Node* currentNeighbour = nullptr;
	std::vector<int>* neighbourList = nullptr;
	Line* line = nullptr;
	
	std::srand(std::time(nullptr));
	int randomWeight = 0;

	for (std::map<int, std::vector<int>*>::iterator it = nodeNeighbours->begin(); it != nodeNeighbours->end(); it++) {
		currentNode = nodesInGraph->at(it->first);
		neighbourList = it->second;
		for (std::vector<int>::iterator nIt = neighbourList->begin(); nIt != neighbourList->end(); nIt++)
		{
			
			currentNeighbour = nodesInGraph->at(*nIt);
			if (!hasConnection(currentNode, currentNeighbour)) {
				if (weighted) {
					randomWeight = (std::rand() % 10);
					line = new Line(currentNode, currentNeighbour, false, false, QString::number(randomWeight));
				}
				else {
					line = new Line(currentNode, currentNeighbour, false, false);
				}
				
				linesInGraph->push_back(line);
			}
		}
	}
}

bool Graph::hasConnection(Node* start, Node* end)
{
	Line* currentLine = nullptr;
	for (std::vector<Line*>::iterator it = linesInGraph->begin(); it != linesInGraph->end(); it++)
	{
		currentLine = *it;
		if ((start == currentLine->getStartNode() && end == currentLine->getEndNode()) || (start == currentLine->getEndNode() && end == currentLine->getStartNode())) {
			return true;
		}
	}
	return false;
}

void Graph::createStaticGraph()
{
	createGraphNode();
	pairNodesByLevel();
	positionGraphNodes();
	createNeighbours();
	createConnections();

	
}

void Graph::createWeightedStaticGraph()
{
	createStaticGraph();
	//for debuggin purposes //temporary hack
	Node* n = nodesInGraph->at(6);
	n->setPos(n->pos().x() + 60, n->pos().y() + 60);
}

/// <summary>
/// TODO: Reftactor later
///Function needs refractorig later
/// </summary>
void Graph::positionGraphNodes(){
	
	Node* firstNode = nullptr;
	Node* nodeToBePositionedLeft = nullptr;
	Node* nodeToBePositionedRight = nullptr;
	QPointF nodePos(0, 0);
	QPointF nodePosRight(0, 0);

	int newPosXLeft = 0;
	int newPosYLeft = 0;
	int newPosXRight = 0;
	int newPosYRight = 0;

	int maxWidthPoint = totalNodesInGraph / 4;
	int totalLevels = maxWidthPoint * 2;

	int midPointX = 0;
	int midPointY = 0;

	std::map<int, std::pair<Node*, Node*>*>::iterator it;
	std::pair<Node*, Node*>* nodePair = nullptr;

	for (int i = 0; i <= totalLevels; i++)
	{
		it = nodePairByLevel->find(i);
		nodePair = it->second;

		if (i <= maxWidthPoint) {
			if (i == 0) {
				firstNode = nodePair->first;
				firstNode->setPos(nodePos);
				midPointX = firstNode->pos().x() + firstNode->boundingRect().width() / 2;
				midPointY = firstNode->pos().y() + firstNode->boundingRect().height();
			}
			else {
				nodeToBePositionedLeft = nodePair->first;
				nodeToBePositionedRight = nodePair->second;
				
				newPosXLeft += (midPointX - 50);
				newPosYLeft += (midPointY + 35);
				nodePos.setX(newPosXLeft);
				nodePos.setY(newPosYLeft);
				nodeToBePositionedLeft->setPos(nodePos);

				newPosXRight += (midPointX + 50);
				newPosYRight += (midPointY + 35);
				nodePosRight.setX(newPosXRight);
				nodePosRight.setY(newPosYRight);
				nodeToBePositionedRight->setPos(nodePosRight);
			}
		}
		else {
			if (i == totalLevels) {
				nodeToBePositionedLeft = nodePair->first;
				nodeToBePositionedRight = nullptr;
				newPosXLeft += (midPointX + 50);
				newPosYLeft += (midPointY + 35);
				nodePos.setX(newPosXLeft);
				nodePos.setY(newPosYLeft);
				nodeToBePositionedLeft->setPos(nodePos);
			}
			else {
				nodeToBePositionedLeft = nodePair->first;
				nodeToBePositionedRight = nodePair->second;

				newPosXLeft += (midPointX + 50);
				newPosYLeft += (midPointY + 35);
				nodePos.setX(newPosXLeft);
				nodePos.setY(newPosYLeft);
				nodeToBePositionedLeft->setPos(nodePos);

				newPosXRight += (midPointX - 50);
				newPosYRight += (midPointY + 35);
				nodePosRight.setX(newPosXRight);
				nodePosRight.setY(newPosYRight);
				nodeToBePositionedRight->setPos(nodePosRight);
			}
		}
	}
}

void Graph::createNeighbours()
{
	//hardCoding neighbours for testing purposes: refractor later

	std::vector<int>* neighbourListNode0 = new std::vector<int>;
	std::vector<int>* neighbourListNode1 = new std::vector<int>;
	std::vector<int>* neighbourListNode2 = new std::vector<int>;
	std::vector<int>* neighbourListNode3 = new std::vector<int>;
	std::vector<int>* neighbourListNode4 = new std::vector<int>;
	std::vector<int>* neighbourListNode5 = new std::vector<int>;
	std::vector<int>* neighbourListNode6 = new std::vector<int>;
	std::vector<int>* neighbourListNode7 = new std::vector<int>;

	neighbourListNode0->push_back(1);
	nodeNeighbours->emplace(0, neighbourListNode0);

	neighbourListNode1->push_back(0);
	neighbourListNode1->push_back(5);
	neighbourListNode1->push_back(6);
	nodeNeighbours->emplace(1, neighbourListNode1);

	neighbourListNode2->push_back(5);
	nodeNeighbours->emplace(2, neighbourListNode2);

	nodeNeighbours->emplace(3, neighbourListNode3);

	neighbourListNode4->push_back(6);
	nodeNeighbours->emplace(4, neighbourListNode4);

	neighbourListNode5->push_back(1);
	neighbourListNode5->push_back(2);
	neighbourListNode5->push_back(7);
	nodeNeighbours->emplace(5, neighbourListNode5);

	neighbourListNode6->push_back(1);
	neighbourListNode6->push_back(4);
	nodeNeighbours->emplace(6, neighbourListNode6);

	neighbourListNode7->push_back(5);
	nodeNeighbours->emplace(7, neighbourListNode7);
}

void Graph::pairNodesByLevel()
{
	int maxWidthPoint = totalNodesInGraph / 4;
	int totalLevels = maxWidthPoint * 2;

	Node* nodeLeft = nullptr;
	Node* nodeRight = nullptr;
	std::pair<Node*, Node*>* nodePairs = nullptr;
	int nodeNo = 0;
	for (int i = 0; i <= totalLevels; i++)
	{
		if (i == 0) {
			nodeLeft = nodesInGraph->at(nodeNo);
			nodeRight = nullptr;
		}
		else if (i == totalLevels) {
			nodeLeft = nodesInGraph->at(totalNodesInGraph - 1);
			nodeRight = nullptr;
		}
		else {
			nodeLeft = nodesInGraph->at(nodeNo - 1);
			nodeRight = nodesInGraph->at(nodeNo);
		}
		nodePairs = new std::pair<Node*, Node*>;
		nodePairs->first = nodeLeft;
		nodePairs->second = nodeRight;
		nodePairByLevel->emplace(i, nodePairs);
		nodeNo += 2;
	}
}



const int Graph::totalNodesInGraph = 8;
