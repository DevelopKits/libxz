#include "stdafx.h"
#include "node.h"
#include "base/logging.h"
#include "rootnode.h"
#include "node_property_map.h"


Node::Node(void)
	: parent_(NULL)
	, first_child_(NULL)
	, last_child_(NULL)
	, next_sibling_(NULL)
	, prev_sibling_(NULL)
	, child_count_(0)
	, property_changed_(false)
{
}


Node::~Node(void)
{
}


Node* Node::GetParent() const
{
	return parent_;
}

Node* Node::GetFirstChild() const
{
	return first_child_;
}

Node* Node::GetLastChild() const
{
	return last_child_;
}

Node* Node::GetPrevSibling() const
{
	return prev_sibling_;
}

Node* Node::GetNextSibling() const
{
	return next_sibling_;
}

void Node::GetAllChildren( std::vector<Node*> &child_array ) const
{
	for (Node* p = first_child_; p!=NULL; p=p->next_sibling_)
	{
		child_array.push_back(p);
	}
}

int32 Node::GetChildCount() const
{
	return child_count_;
}

Node* Node::AppendTo( Node* parent )
{
	return parent->Append(this);
}

Node* Node::Append( Node* child )
{
	return InsertAfter(last_child_, child);
}

Node* Node::Remove( Node* child )
{
	//ֻ�����Ƴ��ӽڵ�
	if(!child || child->parent_ != this)
	{
		NOTREACHED()<<"can only remove child node";
		return NULL;
	}

	//���¸��ӹ�ϵ
	child->parent_ = NULL;
	if (first_child_ == child)
	{
		first_child_ = child->next_sibling_;
	}
	if (last_child_ == child)
	{
		last_child_ = child->prev_sibling_;
	}

	//�����ֵܹ�ϵ
	if(child->prev_sibling_)
	{
		child->prev_sibling_->next_sibling_ = child->next_sibling_;
	}

	if(child->next_sibling_)
	{
		child->next_sibling_->prev_sibling_ = child->prev_sibling_;
	}

	child->next_sibling_ = NULL;
	child->prev_sibling_ = NULL;

	child_count_--;

	return child;
}

Node* Node::InsertAfter( Node* ref, Node* child )
{
	if (ref == child || child == NULL || (ref && ref->parent_ != this) )
	{
		assert(0);
		return NULL;
	}

	//�ȴ�ԭ�ڵ��Ƴ�
	if (child->parent_)
	{
		child->parent_->Remove(child);
	}
	child->parent_ = this;

	child->prev_sibling_ = ref;
	if (ref)
	{//�嵽�м�����
		child->next_sibling_ = ref->next_sibling_;
		if (ref->next_sibling_)
		{
			ref->next_sibling_->prev_sibling_ = child;
		}
		ref->next_sibling_ = child;

		DCHECK(last_child_);
		//�嵽�����
		if (last_child_->next_sibling_)
		{
			last_child_ = last_child_->next_sibling_;
		}
	}
	else
	{//�嵽��ǰ
		child->next_sibling_ = first_child_;
		if (first_child_)
		{
			first_child_->prev_sibling_ = child;
		}
		first_child_ = child;

		//ԭ��û����Ԫ��
		if (last_child_ == NULL)
		{
			last_child_ = child;
		}
	}
	child_count_++;

	return child;
}

Node* Node::InsertBefore( Node* ref, Node* child )
{
	if (ref == child || child == NULL || (ref && ref->parent_ != this) )
	{
		NOTREACHED()<<"insert before invalid";
		return NULL;
	}

	if (ref)
	{
		return InsertAfter(ref->prev_sibling_, child);
	}
	else
	{
		return InsertAfter(last_child_, child);
	}
}

bool Node::PreVisit( std::function<bool(Node*)> func )
{
	if (!func(this))
		return false;

	for (Node* p = first_child_; p!=NULL; p=p->next_sibling_)
	{
		if (!p->PreVisit(func))
			break;
	}

	return true;
}


bool Node::PostVisit( std::function<bool(Node*)> func )
{
	for (Node* p = first_child_; p!=NULL; p=p->next_sibling_)
	{
		if (!p->PostVisit(func))
			break;
	}

	if (!func(this))
		return false;

	return true;
}



Node* Node::Detach()
{
	//RootNode* root = GetRoot();
	return parent_->Remove(this);
}

RootNode* Node::GetRoot() const
{
	const Node* p = this;
	while(p->parent_)
		p = p->parent_;
	return dynamic_cast<RootNode*>(const_cast<Node*>(p));
}

void Node::SetProperty( const std::string name, const std::wstring& value )
{
	if (!property_map_.get())
	{
		property_map_.reset(new NodePropertyMap);
	}
	property_map_->SetProperty(name, value);
	property_changed_ = true;
}

bool Node::IsPropertyChanged() const
{
	return property_changed_;
}

void Node::ReadProperties()
{

	ReadNodeProperty(property_map_.get());
	property_changed_ = false;
}

bool Node::GetProperty( const std::string name, std::wstring& value )
{
	if (!property_map_.get())
		return false;

	return property_map_->GetProperty(name, value);
}

void Node::ReadNodeProperty(NodePropertyMap* property_map)
{
	property_map->GetString("id", id_);
	property_changed_ = false;
}

void Node::WriteNodeProperty( NodePropertyMap* property_map )
{
	property_map_.reset(property_map);
	property_changed_ = true;
}




