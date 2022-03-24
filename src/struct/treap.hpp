#pragma once

#include<iostream>

struct TreapNode {
    TreapNode* LeftChild;
    TreapNode* RightChild;
    TreapNode* Parent;
    int Key;
	int Priority;
	TreapNode() = default;
    TreapNode(int Key, int Priority) : LeftChild(nullptr), RightChild(nullptr), Parent(nullptr), Key(Key), Priority(Priority) {};
};

struct Treap {
	TreapNode *root;
	bool usedPriority[RAND_MAX];
    Treap() = default;
    int SetNodePriority();
	void LeftRotate(TreapNode *);
	void RightRotate(TreapNode *);
	void DeleteNoOrOneChildTPNode(TreapNode *,TreapNode *);
	void ModifyTreap(TreapNode *);
	void InsertTreap(int);
	bool DeleteTreap(int);
	bool UpdataTreap(int,int);
	TreapNode *FindTreap(int);

    void PreOrderTPPrint(TreapNode *);
	void InOrderTPPrint(TreapNode *);
	void RotateTPPrint(TreapNode *,int);
	void SufOrderTPPrint(TreapNode *);
	void PreOrderTPPrint();
	void InOrderTPPrint();
	void RotateTPPrint();
	void SufOrderTPPrint();
};

int Treap::SetNodePriority()
{
	int val;
	while(usedPriority[val=rand()]);
	usedPriority[val]=true;
	return val;
}

void Treap::LeftRotate(TreapNode *Node)
{
	TreapNode *rChild=Node->RightChild;
	if(NULL==rChild) {
        return;
    }
	if(NULL!=Node->Parent)//不为根节点
	{
		if(Node->Parent->LeftChild==Node) {
            Node->Parent->LeftChild=rChild;
        }
		else {
            Node->Parent->RightChild=rChild;
        }
	}
	rChild->Parent=Node->Parent;
	Node->Parent=rChild;
	if(rChild->LeftChild!=NULL){
        rChild->LeftChild->Parent=Node;
    }
	Node->RightChild=rChild->LeftChild;
	rChild->LeftChild=Node;
	if(NULL==rChild->Parent) {
            this->root=rChild;
    }
}

void Treap::RightRotate(TreapNode* Node)
{
	TreapNode *lChild = Node->LeftChild;
	if(lChild == nullptr) {
        return;
    }
	if(Node->Parent != nullptr)//不为根节点
	{
		if(Node->Parent->RightChild == Node) {
            Node->Parent->RightChild=lChild;
        }
		else {
            Node->Parent->LeftChild=lChild;
        }
	}
	lChild->Parent = Node->Parent;
	Node->Parent = lChild;
	if(lChild->RightChild != nullptr) {
        lChild->RightChild->Parent=Node;
    }
	Node->LeftChild = lChild->RightChild;
	lChild->RightChild = Node;
	if(lChild->Parent == nullptr) {
        this->root=lChild;
    }
}

void Treap::ModifyTreap(TreapNode *Node)
{
	TreapNode* ParentNode = Node->Parent;
	while(ParentNode != nullptr && ParentNode->Priority > Node->Priority)//大于父亲节点优先级
	{
		if(Node == ParentNode->LeftChild) {
            RightRotate(ParentNode);
        }
		else {
            LeftRotate(ParentNode);
        }
		ParentNode=Node->Parent;
	}
}

void Treap::InsertTreap(int key)
{
	TreapNode* pre = nullptr;
    TreapNode* cur = this->root;
	while(cur != nullptr) {
		pre = cur;
		if(cur->Key > key) {
            cur = cur->LeftChild;
        }
		else {
            cur = cur->RightChild;
        }
	}
	TreapNode* node = new TreapNode(key, SetNodePriority());
	node->Parent = pre;
	if(pre == nullptr) {//若插入的为根节点
		this->root = node;
	}
	else if(pre->Key > node->Key) {
        pre->LeftChild = node;
    }
	else {
        pre->RightChild = node;
    }
	ModifyTreap(node);
}

TreapNode *Treap::FindTreap(int Key)
{
	TreapNode *cur=this->root;
	while(cur != nullptr) {
		if(cur->Key == Key) {
            break;
        } else if(cur->Key > Key) {
            cur=cur->LeftChild;
        } else {
            cur=cur->RightChild;
        }
	}
	return cur;
}

void Treap::DeleteNoOrOneChildTPNode(TreapNode *pre, TreapNode *cur)
{
	if(cur->LeftChild == nullptr && cur->RightChild == nullptr) {//左右孩子为空
		if(pre == nullptr) {
            this->root=nullptr;
        }
		else if(pre->LeftChild == cur) {
            pre->LeftChild=nullptr;
        }
		else {
            pre->RightChild=nullptr;
        }
		delete cur;
	} else if(cur->RightChild != nullptr ) {
		if(pre == nullptr) { 
            this->root = cur->RightChild;
            cur->RightChild->Parent = nullptr ;
		} else if(pre->LeftChild == cur) {
			pre->LeftChild = cur->RightChild;
			cur->RightChild->Parent = pre;
		} else {
            pre->RightChild = cur->RightChild;
            cur->RightChild->Parent = pre;
		}
		delete cur;
	} else if(cur->LeftChild != nullptr ) {//若左子树不为空
		if(pre == nullptr) {
            this->root = cur->LeftChild;
            cur->LeftChild->Parent = nullptr;
		} else if(pre->LeftChild == cur) {
            pre->LeftChild = cur->LeftChild;
            cur->LeftChild->Parent = pre;
		} else {
            pre->RightChild=cur->LeftChild;
            cur->LeftChild->Parent=pre;
		}
        delete cur;
    }
}

bool Treap::DeleteTreap(int Key)
{
	TreapNode* pre = nullptr;
    TreapNode* cur = root;
	while(cur != nullptr)//寻找待删除元素
	{
		if(cur->Key == Key) {
            break;
        } else {
            pre=cur;
			if(cur->Key > Key) {
                cur = cur->LeftChild;
            } else {
                cur = cur->RightChild;
            }
		}
	}
	if(cur == nullptr){
        return false;
    }
	if(cur->LeftChild == nullptr || cur->RightChild == nullptr) {
        DeleteNoOrOneChildTPNode(pre, cur);
    } else {//左右子树都不为空
        TreapNode* rPre = cur;
        TreapNode* rCur = cur->RightChild;//找到右子树最小元素
		bool isLeft;
		while(rCur->LeftChild != nullptr) {
            rPre = rCur;
            rCur = rCur->LeftChild;
		}
		cur->Key = rCur->Key;//直接填充元素，不复制优先级
		if(rPre->LeftChild == rCur) {
            isLeft = true;
        } else {
            isLeft = false;
        }
		DeleteNoOrOneChildTPNode(rPre, rCur);
		if(isLeft) {
            ModifyTreap(rPre->LeftChild);
        } else {
            ModifyTreap(rPre->RightChild);
        }
	}
	return true;
}

bool Treap::UpdataTreap(int oldKey,int newKey)
{
	if(DeleteTreap(oldKey)) {
		InsertTreap(newKey);
		return true;
	}
	return false;
}



void Treap::PreOrderTPPrint(TreapNode *tempTPNode)
{
	if(NULL==tempTPNode)
		return ;
	std::cout<<"("<<tempTPNode->Key<<","<<tempTPNode->Priority<<")   ";
	PreOrderTPPrint(tempTPNode->LeftChild);
	PreOrderTPPrint(tempTPNode->RightChild);
}
void Treap::PreOrderTPPrint()
{
	PreOrderTPPrint(this->root);
}
void Treap::InOrderTPPrint(TreapNode *tempTPNode)
{
	if(NULL==tempTPNode)
		return ;
	InOrderTPPrint(tempTPNode->LeftChild);
	std::cout<<"("<<tempTPNode->Key<<","<<tempTPNode->Priority<<")   ";
	InOrderTPPrint(tempTPNode->RightChild);
}
void Treap::InOrderTPPrint()
{
	InOrderTPPrint(this->root);
}
void Treap::SufOrderTPPrint(TreapNode *tempTPNode)
{
	if(NULL==tempTPNode)
		return ;
	SufOrderTPPrint(tempTPNode->LeftChild);
	SufOrderTPPrint(tempTPNode->rightChild);
	std::cout<<"("<<tempTPNode->Key<<","<<tempTPNode->riority<<")   ";
}
void Treap::SufOrderTPPrint()
{
	SufOrderTPPrint(this->root);
}
void Treap::RotateTPPrint(TreapNode *tempTPNode,int tempColumn)
{
	if(NULL==tempTPNode)
		return ;
	RotateTPPrint(tempTPNode->LeftChild,tempColumn+1);
	for(int i=0;i<tempColumn;i++)
		std::cout<<"    ";
	std::cout<<"("<<tempTPNode->Key<<","<<tempTPNode->Priority<<")"<<std::endl;
	RotateTPPrint(tempTPNode->RightChild,tempColumn+1);
}
void Treap::RotateTPPrint()
{
	RotateTPPrint(this->root,0);
}