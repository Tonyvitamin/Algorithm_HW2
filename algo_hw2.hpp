// change this to your id
static const char* student_id = "0416001";


// do not edit prototype
void Insert(int *, int);
void Delete(int *, int);
int Select(int *, int);
int Rank(int *, int);

// data structure :
// tree is an array with enough space
// tree[0] is the array size
// bundle three attribute as a node data
// First attribute is color, "0" means black, "1" means red , NIL is also "0", "-1" means no data
// Second attribute is key, "0" means NIL, "1"~"999" means data,  "-1" means no data
// Third attribute is size, for Dynamic Order Statistics, "0" means NIL , "-1" means no data
//
// for example,
// if tree[0] is "256" says that the array size is 256
//
// if tree[1] is "1" says that the place of 1 is a red node
//
// if tree[2] is "5" says that the place of 1 is key of 5
//
// if tree[8] is "-1" says that the place of 3 has nothing
//
// if tree[14] is "0" says that the place of 5 is a node of NIL
//
// if tree[19] is "66" says that the place of 7 is key of 66

//
// if there is an ambiguous situation, choose the smaller ( left ) one
//

//
// do your homework here
//
#define black 0
#define red 1
#include <iostream>

struct rb_tree {
	int color;
	int key;
	int original_color;
	rb_tree * parent;
	rb_tree * left_child;
	rb_tree * right_child;
	rb_tree() : color(black), key(0), original_color(black), parent(NULL), left_child(NULL), right_child(NULL) {};
};

class rb {
public:
	rb();
	int	inorder_travel(rb_tree * node, int index, int * tree);
	void	insert_rb(rb_tree * node);
	void 	insert_rb_fix(rb_tree * node);
	void	left_rotate(rb_tree * node);
	void	right_rotate(rb_tree * node);
	void	RB_transplant(rb_tree * u, rb_tree * v);
	int		RB_delete(int key);
	void	rb_delete_fix(rb_tree* node);
private:
	rb_tree * NIL;
	rb_tree * root;
};


rb::rb()
{
	NIL = new rb_tree();
	root = new rb_tree();
	root = NIL;
	root->color = black;
	root->key = -1;
	root->parent = NIL;
	root->left_child = NIL;
	root->right_child = NIL;
}


int  rb::inorder_travel(rb_tree * node, int index, int *tree)
{
	//handle root case //
	if (index == 1)
		node = this->root;
	//terminate condition//
	if (node == this->NIL)
	{
		tree[3 * index - 2] = 0;
		tree[3 * index - 1] = 0;
		tree[3 * index] = 0;
		return index;
	}

	int a, b, c;
	//search right side//
	a = inorder_travel(node->left_child, index * 2, tree);
	tree[3 * index - 1] = node->key;
	tree[3 * index - 2] = node->color;

	//search in left side//
	b = inorder_travel(node->right_child, index * 2 + 1, tree);
	tree[3 * index - 1] = node->key;
	tree[3 * index - 2] = node->color;

	//compare which child is deeper//
	if (a > b)
		c = a;
	else c = b;
	delete node;
	return c;
}
void rb::left_rotate(rb_tree * x)
{
	rb_tree * y = x->right_child;
	x->right_child = y->left_child;

	if (y->left_child != this->NIL)
		y->left_child->parent = x;

	y->parent = x->parent;

	if (x->parent == this->NIL)
		this->root = y;
	else if (x == x->parent->left_child)
		x->parent->left_child = y;
	else x->parent->right_child = y;

	y->left_child = x;
	x->parent = y;
}
void rb::right_rotate(rb_tree * x)
{
	rb_tree * y = x->left_child;
	x->left_child = y->right_child;

	if (y->right_child != this->NIL)
		y->right_child->parent = x;

	y->parent = x->parent;

	if (x->parent == this->NIL)
		this->root = y;
	else if (x == x->parent->left_child)
		x->parent->left_child = y;
	else x->parent->right_child = y;

	y->right_child = x;
	x->parent = y;
}
void rb::insert_rb_fix(rb_tree * node)
{
	while (node->parent->color == red)
	{

		if (node->parent == node->parent->parent->left_child)
		{
			rb_tree * y = node->parent->parent->right_child;
			if (y->color == red)
			{
				node->parent->color = black;
				y->color = black;
				node->parent->parent->color = red;
				node = node->parent->parent;
			}
			else
			{
				if (node == node->parent->right_child)
				{
					node = node->parent;
					left_rotate(node);
				}
				node->parent->color = black;
				node->parent->parent->color = red;
				right_rotate(node->parent->parent);
			}
		}
		else
		{
			rb_tree * y = node->parent->parent->left_child;
			if (y->color == red)
			{
				node->parent->color = black;
				y->color = black;
				node->parent->parent->color = red;
				node = node->parent->parent;
			}
			else
			{
				if (node == node->parent->left_child)
				{
					node = node->parent;
					right_rotate(node);
				}
				node->parent->color = black;
				node->parent->parent->color = red;
				left_rotate(node->parent->parent);
			}
		}
	}
	this->root->color = black;
}
void rb::insert_rb(rb_tree * node)
{
	rb_tree * y = this->NIL;
	rb_tree * x = this->root;
	while (x != this->NIL)
	{
		y = x;
		if (node->key < x->key)
			x = x->left_child;
		else
			x = x->right_child;
	}
	node->parent = y;

	if (y == this->NIL)
		this->root = node;

	else if (node->key < y->key)
		y->left_child = node;

	else y->right_child = node;

	node->color = red;
	node->right_child = this->NIL;
	node->left_child = this->NIL;

	insert_rb_fix(node);
	return;
}
///////Used for Red_black_tree///////////////
void rb::RB_transplant(rb_tree * u, rb_tree * v)
{
	if (u->parent == this->NIL)
		this->root = v;
	else if (u == u->parent->left_child)
		u->parent->left_child = v;
	else u->parent->right_child = v;
	v->parent = u->parent;
}
void rb::rb_delete_fix(rb_tree * node)
{
	while (node != root && node->color == black)
	{
		if (node == node->parent->left_child)
		{
			rb_tree * w;// = new rb_tree();
			w = node->parent->right_child;
			if (w->color == red)
			{
				w->color = black;
				node->parent->color = red;
				left_rotate(node->parent);
				w = node->parent->right_child;
			}
			if (w->left_child->color == black && w->right_child->color == black)
			{
				w->color = red;
				node = node->parent;
			}
			else
			{
				if (w->right_child->color == black)
				{
					w->left_child->color = black;
					w->color = red;
					right_rotate(w);
					w = node->parent->right_child;
				}
				w->color = node->parent->color;
				node->parent->color = black;
				w->right_child->color = black;
				left_rotate(node->parent);
				node = root;
			}
		}
		else
		{
			rb_tree * w;// = new rb_tree();
			w = node->parent->left_child;
			if (w->color == red)
			{
				w->color = black;
				node->parent->color = red;
				right_rotate(node->parent);
				w = node->parent->left_child;
			}
			if (w->left_child->color == black && w->right_child->color == black)
			{
				w->color = red;
				node = node->parent;
			}
			else
			{
				if (w->left_child->color == black)
				{
					w->right_child->color = black;
					w->color = red;
					left_rotate(w);
					w = node->parent->left_child;
				}
				w->color = node->parent->color;
				node->parent->color = black;
				w->left_child->color = black;
				right_rotate(node->parent);
				node = root;
			}
		}
	}
	node->color = black;
}
int  rb::RB_delete(int key)
{
	rb_tree * tmp = this->root;
	while (tmp != NIL && key != tmp->key)
	{
		if (key < tmp->key)
			tmp = tmp->left_child;
		else if (key > tmp->key)
			tmp = tmp->right_child;
	}
	if (tmp == this->NIL)
		return 0;

	rb_tree * z = tmp;

	rb_tree * y = new rb_tree();
	y = z;
	int original_color = y->color;

	rb_tree * x = new rb_tree();

	if (z->left_child == this->NIL)
	{
		x = z->right_child;
		RB_transplant(z, z->right_child);
	}
	else if (z->right_child == this->NIL)
	{
		x = z->left_child;
		RB_transplant(z, z->left_child);
	}
	else
	{
		rb_tree * a = z->left_child;
		while (a != this->NIL)
			a = a->right_child;
		y = a;
		original_color = y->color;
		x = y->right_child;
		if (y->parent == z)
			x->parent = y;
		else
		{
			RB_transplant(y, y->right_child);
			y->right_child = z->right_child;
			y->right_child->parent = y;
		}
		RB_transplant(z, y);
		y->left_child = z->left_child;
		y->left_child->parent = y;
		y->color = z->color;
	}
	if (original_color == black)
		rb_delete_fix(x);

	delete x;
	delete y;
	return 1;
}

void Insert(int * tree, int key) {

	rb T;
	int tree_size = tree[0];
	for (int i = 2, n = 2; n<tree_size; i++)
	{
		if (tree[n]>0)
		{
			rb_tree * node = new rb_tree();
			node->key = tree[n];
			T.insert_rb(node);
		}
		n = i * 3 - 1;
	}
	//insert new node
	rb_tree * node = new rb_tree();
	node->key = key;
	T.insert_rb(node);

	//wrire back to tree
	rb_tree * tmp = new rb_tree();
	T.inorder_travel(tmp, 1, tree);
	//delete tmp;
	return;
}
void Delete(int * tree, int key) {
	rb T;
	int tree_size = tree[0];

	//insert tree into rb tree
	for (int i = 2, n = 2; n<tree_size; i++)
	{
		if (tree[n]>0)
		{
			rb_tree * node = new rb_tree();
			node->key = tree[n];
			T.insert_rb(node);
		}
		n = i * 3 - 1;
	}

	int check = T.RB_delete(key);
	rb_tree * useless_node = new rb_tree();

	if (check == 1)//if there is any node or nothing in the tree
	{
		int n;
		n = T.inorder_travel(useless_node, 1, tree);
		//for all remaining nodes
		for (int index = 3 * (n + 1); index < tree_size;)
		{
			tree[index - 2] = -1;
			tree[index - 1] = -1;
			tree[index] = -1;
			n++;
			index = 3 * n;
		}
	}
	//delete useless_node;
}

int Select(int * tree, int i) {
	// use Dynamic Order Statistics to return the i'th smallest element


	return -1;
}

int Rank(int * tree, int x) {
	// use Dynamic Order Statistics to return the rank of element x in the tree


	return -1;
}
