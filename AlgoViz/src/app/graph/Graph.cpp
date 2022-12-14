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
	this->weightMatrix = new std::vector<std::vector<int>*>;
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

std::vector<std::vector<int>*>* Graph::getWeightMatrix()
{
	return this->weightMatrix;
}

Line* Graph::getConnectingLine(Node* start, Node* end)
{
	Line* currentLine = nullptr;
	for (std::vector<Line*>::iterator it = linesInGraph->begin(); it != linesInGraph->end(); it++)
	{
		currentLine = *it;
		if ((start == currentLine->getStartNode() && end == currentLine->getEndNode()) || (start == currentLine->getEndNode() && end == currentLine->getStartNode())) {
			return currentLine;
		}
	}
	return nullptr;
}

std::vector<std::vector<int>> Graph::getEdgeList() {
	return this->edgeList;
}
std::vector<std::vector<int>> Graph::getEdgeListOneWay() {
	return this->edgeListOneWay;
}

void Graph::createGraphNode()
{
	Node* graphNode = nullptr;
	for (int i = 0; i < TOTAL_NODES_IN_GRAPH; i++)
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
					randomWeight = (std::rand() % 9) + 1;
					line = new Line(currentNode, currentNeighbour, false, false, QString::number(randomWeight));
					setWeightMatrixVal(currentNode, currentNeighbour, randomWeight);
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
	//createNeighbours();
	createRandomNeighbours();
	initializeWeightMatrix();
	createConnections();
}

void Graph::createWeightedStaticGraph()
{
	createStaticGraph();
	//for debugging purposes //temporary hack
	/*Node* n = nodesInGraph->at(6);
	n->setPos(n->pos().x() + 60, n->pos().y() + 60);

	n = nodesInGraph->at(4);
	n->setPos(n->pos().x() + 120, n->pos().y() + 20);*/

	createEdgeList();
	createEdgeListOneWay();
}

void Graph::initializeWeightMatrix()
{	
	std::vector<int>* rowVector = nullptr;
	int totalNodesInCurrentGraph = nodesInGraph->size();
	for (int i = 0; i < TOTAL_NODES_IN_GRAPH; i++)
	{
		rowVector = new std::vector<int>(TOTAL_NODES_IN_GRAPH, 0);
		this->weightMatrix->emplace_back(rowVector);
	}
}

void Graph::setWeightMatrixVal(Node* startNode, Node*neighbourNode, int weight)
{
	int currentNodeVal = startNode->getNodeValInt();
	int neighbourNodeVal = neighbourNode->getNodeValInt();
	this->weightMatrix->at(currentNodeVal)->at(neighbourNodeVal) = weight;
	this->weightMatrix->at(neighbourNodeVal)->at(currentNodeVal) = weight;
}

void Graph::createEdgeList() {
	int weight = 0;
	Node* currentNode = nullptr;
	Node* nextNode = nullptr;
	for (int row = 0; row < weightMatrix->size(); row++)
	{
		currentNode = this->nodesInGraph->at(row);
		for (int col = 0; col < weightMatrix->at(row)->size(); col++) {
			nextNode = this->nodesInGraph->at(col);
			if (hasConnection(currentNode, nextNode)) {
				weight = weightMatrix->at(row)->at(col);
				edgeList.push_back({ row, col, weight });
			}
		}
	}
}

void Graph::createEdgeListOneWay() {
	int edgeI = -1;
	int edgeII = -1;
	int weight = -1;
	for(std::vector<std::vector<int>>::iterator it = edgeList.begin(); it != edgeList.end();  it++)
	{
		edgeI = (*it).at(0);
		edgeII = (*it).at(1);
		weight = (*it).at(2);
		if (!edgeAlreadyInList(edgeII, edgeII, weight)) {
			edgeListOneWay.push_back({ edgeI, edgeII, weight });
		}
	}
}

bool Graph::edgeAlreadyInList(int edgeI, int edgeII, int weight) {
	for (std::vector<std::vector<int>>::iterator it = edgeListOneWay.begin(); it != edgeListOneWay.end();  it++)
	{
		if (edgeI == (*it).at(0) && edgeII == (*it).at(1) && weight == (*it).at(2)) {
			return true;
		}
	}
	return false;
}

// TODO: Reftactor later
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

	int maxWidthPoint = TOTAL_NODES_IN_GRAPH / 4;
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
				
				newPosXLeft += (midPointX - 110);
				newPosYLeft += (midPointY + 35);
				nodePos.setX(newPosXLeft);
				nodePos.setY(newPosYLeft);
				nodeToBePositionedLeft->setPos(nodePos);

				newPosXRight += (midPointX + 90);
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
				newPosXLeft += (midPointX + 110);
				newPosYLeft += (midPointY + 35);
				nodePos.setX(newPosXLeft);
				nodePos.setY(newPosYLeft);
				nodeToBePositionedLeft->setPos(nodePos);
			}
			else {
				nodeToBePositionedLeft = nodePair->first;
				nodeToBePositionedRight = nodePair->second;

				newPosXLeft += (midPointX + 110);
				newPosYLeft += (midPointY + 35);
				nodePos.setX(newPosXLeft);
				nodePos.setY(newPosYLeft);
				nodeToBePositionedLeft->setPos(nodePos);

				newPosXRight += (midPointX - 90);
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
	neighbourListNode1->push_back(2); //added later to make loop in graph
	neighbourListNode1->push_back(5);
	neighbourListNode1->push_back(6);
	nodeNeighbours->emplace(1, neighbourListNode1);

	neighbourListNode2->push_back(1); //added later to make loop in graph
	neighbourListNode2->push_back(4); //added later to make loop in graph
	neighbourListNode2->push_back(5);
	nodeNeighbours->emplace(2, neighbourListNode2);

	nodeNeighbours->emplace(3, neighbourListNode3);

	neighbourListNode4->push_back(2); //added later to make loop in graph
	neighbourListNode4->push_back(5); //added later to make loop in graph
	neighbourListNode4->push_back(6);
	nodeNeighbours->emplace(4, neighbourListNode4);

	neighbourListNode5->push_back(1);
	neighbourListNode5->push_back(2);
	neighbourListNode5->push_back(4); //added later to make loop in graph
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
	int maxWidthPoint = TOTAL_NODES_IN_GRAPH / 4;
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
			nodeLeft = nodesInGraph->at(TOTAL_NODES_IN_GRAPH - 1);
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

int Graph::randomValInRangeInclusive(int lowerBound, int upperBound) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distr(lowerBound, upperBound);
	return distr(gen);
}

bool Graph::valIsInList(std::vector<int>& list, int val)
{
	for (std::vector<int>::iterator it = list.begin(); it != list.end(); it++) {
		if (val == (*it)) {
			return true;
		}
	}
	return false;
}

void Graph::createRandomNeighbours() {
	this->possibleLoneNodes = randomValInRangeInclusive(MIN_POSSIBLE_LONE_NODES, MAX_POSSIBLE_LONE_NODES);
	this->possibleNodeLines = randomValInRangeInclusive(MIN_POSSIBLE_NODE_LINES, (MAX_POSSIBLE_NODE_LINES - this->possibleLoneNodes));

	std::vector<int>* blackListedNodes = new std::vector<int>();

	std::srand(time(nullptr));
	while (blackListedNodes->size() != this->possibleLoneNodes)
	{
		int randomNode = std::rand() % TOTAL_NODES_IN_GRAPH;
		if (!valIsInList(*blackListedNodes, randomNode) && randomNode != 0) {
			blackListedNodes->push_back(randomNode);
		}
	}
	
	std::map<int, std::vector<int>*>* possibleNeighboursForAllNodes = generatePossibleNeighboursList();
	

	int numOfNeighboursConnected = 0;
	int randomStartNode = -1;
	int randomNeighbourSelected = -1;
	std::vector<int>* neighbourList = nullptr;


	for (int i = 0; i < nodesInGraph->size(); i++)
	{
		neighbourList = new std::vector<int>;
		nodeNeighbours->emplace(i, neighbourList);
	}

	while (numOfNeighboursConnected != this->possibleNodeLines)
	{
		randomStartNode = std::rand() % (TOTAL_NODES_IN_GRAPH - this->possibleLoneNodes);
		if (!valIsInList(*blackListedNodes, randomStartNode)) {
			std::random_device rd;
			std::mt19937 eng(rd());
			std::uniform_int_distribution<> distr(0, possibleNeighboursForAllNodes->at(randomStartNode)->size() - 1);
			randomNeighbourSelected = possibleNeighboursForAllNodes->at(randomStartNode)->at(distr(eng));

			Node* firstNode = nodesInGraph->at(randomStartNode);
			Node* secondNode = nodesInGraph->at(randomNeighbourSelected);
			if (!hasConnection(firstNode, secondNode)) {
				nodeNeighbours->at(randomStartNode)->push_back(randomNeighbourSelected);
				nodeNeighbours->at(randomNeighbourSelected)->push_back(randomStartNode);
				numOfNeighboursConnected++;
			}
		}

	}
}

std::map<int, std::vector<int>*>* Graph::generatePossibleNeighboursList() {
	

	std::map<int, std::vector<int>*>* possibleNeighboursListForAllNodes = new std::map<int, std::vector<int>*>;
	std::vector<int>* possibleNeighbours = nullptr;
	for (int i = 0; i < nodesInGraph->size(); i++)
	{
		possibleNeighbours = new std::vector<int>;
		possibleNeighboursListForAllNodes->emplace(i, possibleNeighbours);
	}

	//hardcoding ... TODO: refactor later, make it dynamic
	std::vector<int>::iterator it = possibleNeighboursListForAllNodes->at(0)->begin();
	possibleNeighboursListForAllNodes->at(0)->insert(it, { 1,2,5});
	
	it = possibleNeighboursListForAllNodes->at(1)->begin();
	possibleNeighboursListForAllNodes->at(1)->insert(it, { 0,2,3,4,5,6,7});

	it = possibleNeighboursListForAllNodes->at(2)->begin();
	possibleNeighboursListForAllNodes->at(2)->insert(it, { 0,1,3,4,5,6,7 });

	it = possibleNeighboursListForAllNodes->at(3)->begin();
	possibleNeighboursListForAllNodes->at(3)->insert(it, {1,2,4,5,6});

	it = possibleNeighboursListForAllNodes->at(4)->begin();
	possibleNeighboursListForAllNodes->at(4)->insert(it, { 1,2,3,5,6});

	it = possibleNeighboursListForAllNodes->at(5)->begin();
	possibleNeighboursListForAllNodes->at(5)->insert(it, { 0,1,2,3,4,6,7 });

	it = possibleNeighboursListForAllNodes->at(6)->begin();
	possibleNeighboursListForAllNodes->at(6)->insert(it, { 0,1,2,3,4,5,7 });

	it = possibleNeighboursListForAllNodes->at(7)->begin();
	possibleNeighboursListForAllNodes->at(7)->insert(it, { 0,1,2,5,6 });

	return possibleNeighboursListForAllNodes;
}


const int Graph::TOTAL_NODES_IN_GRAPH = 8;
const int Graph::MIN_POSSIBLE_LONE_NODES = 0;
const int Graph::MAX_POSSIBLE_LONE_NODES = 1;
const int Graph::MIN_POSSIBLE_NODE_LINES = 7;
const int Graph::MAX_POSSIBLE_NODE_LINES = 9;
