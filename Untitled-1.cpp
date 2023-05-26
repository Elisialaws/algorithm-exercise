#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
using namespace std;
class rect;
class kdtree;
class point{
    public:
    point(double a,double b):x(a),y(b){}
    point (const point &p){
        x=p.x;
        y=p.y;
    }
    double distance(point &that){
        double dx=x-that.x;
        double dy=y-that.y;
        return dx*dx+dy*dy;
    }
    friend class rect;
    friend class kdtree;
    double getx()const{return x;}
    double gety()const{return y;}
    private:
    double x;
    double y;
    };
class rect{
    public:
    rect(double x1,double y1,double x2,double y2):xmin(x1),ymin(y1),xmax(x2),ymax(y2){}
    bool contain(point &p){
        return (p.x>=xmin)&&(p.y>=ymin)&&(p.x<=xmax)&&(p.y<=ymax);
    }
    double distance(point &p){
        double dx=0.0,dy=0.0;
       if      (p.x < xmin) dx = p.x - xmin;
        else if (p.x > xmax) dx = p.x - xmax;
        if      (p.y < ymin) dy = p.y - ymin;
        else if (p.y > ymax) dy = p.y - ymax;
        return dx*dx + dy*dy;
    }
    bool intersect(rect &that){
         return (this->xmax >= that.xmin) && (this->ymax >= that.ymin) &&
               (that.xmax >= this->xmin) && (that.ymax >= this->ymin);
    }
private:
double xmin,ymin;
double xmax,ymax;
};
class kdtree{
   private:
    class node{
        public:
        point p;
        rect r;
        node *leftbottom;
        node *righttop;
        node *root;
        node(point p,rect r):p(p),r(r),leftbottom(nullptr),righttop(nullptr){}

    };
    node *root;
    int size;
    public:
    kdtree():root(nullptr),size(0){}
    bool empty(){return size==0;}
    int number(){return size;}
    void insert(point p){
        if(root==nullptr){
            root=new node(p,rect(0,0,1,1));
            ++size;
        }
        else{
            insert(root,p,true,0.0,0.0,1.0,1.0);//先定x后定y
        }
    }
     bool contain(point &p){
return contain(root,p,true);
    }
    vector<point> range(rect & r){
        vector<point> points;
        range(root,r,points);
        return points;
    }
    point &near(point &p){
        point t(0,0);
        return near(root,t,true,p);
    }
    private:
    node* insert(node *t,point p,bool flag,double xmin,double ymin,double xmax,double ymax){ 
        if(t==nullptr){
            ++size;
            return new node(p,rect(xmin,ymin,xmax,ymax));
        }
        if(t->p.x==p.x&&t->p.x==p.y) return t; 
        if(flag){
            if(p.x<t->p.x){
                t->leftbottom=insert(t->leftbottom,p,!flag,xmin,ymin,t->p.x,ymax);
            }else{
                t->righttop=insert(t->righttop,p,!flag,t->p.x,ymin,xmax,ymax);
            }
        }
        else{if(t->p.y>p.y){
t->leftbottom=insert(t->leftbottom,p,!flag,xmin,ymin,xmax,t->p.y);
        }else t->righttop=insert(t->righttop,p,!flag,xmin,t->p.y,xmax,ymax);
    }}
  bool contain(node *head,point &p,bool flag){
    if(head==nullptr)return false;
    if(head->p.x==p.x&&head->p.y==p.y)return true;
    if(flag){
if(head->p.x>p.x){
    contain(head->leftbottom,p,!flag);
}else contain(head->righttop,p,!flag);
    }else{
        if(head->p.y>p.y){
           contain(head->leftbottom,p,!flag);
        }else contain(head->righttop,p,!flag);
    }
   }
void range(node *head,rect &r,vector<point> &points){
    if(head==nullptr)return;
if(r.intersect(head->r)){
    if(r.contain(head->p)) points.push_back(head->p);
    range(head->leftbottom,r,points);
    range(head->righttop,r,points);
}
}
point& near(node *head,point &mins,bool flag,point &p){
    if(head==nullptr)return mins;
    if(mins.distance(p)>head->p.distance(p)||(mins.x==0&&mins.y==0)){
        mins=head->p;
    }
    if(flag){
        if(p.x<head->p.x){
            near(head->leftbottom,mins,!flag,p);
        }else{
            near(head->righttop,mins,!flag,p);
        }}else{
            if(p.y<head->p.y){
                near(head->leftbottom,mins,!flag,p);
            }else near(head->righttop,mins,!flag,p);
        }return mins;
    }
};
int main(){
    kdtree tree;
    tree.insert(point(0.5,0.5));
    tree.insert(point(0.3,0.7));
    tree.insert(point(0.5,0.4));
    tree.insert(point(0.2,0.3));
    tree.insert(point(0.9,0.1));
    tree.insert(point(0.9,0.8));
    rect r(0.0,0.0,1.0,1.0);
    vector<point> points=tree.range(r);
    for(point p:points){
        cout<<p.getx()<<","<<p.gety()<<endl;
    }
 point p(0.8,0.7);
 point nearest=tree.near(p);
 cout<<nearest.getx()<<","<<nearest.gety()<<endl;
   

}

/*struct Point2D {
    double x;
    double y;

    Point2D(double x, double y) : x(x), y(y) {}

    double distanceSquaredTo(Point2D that) {
        double dx = this->x - that.x;
        double dy = this->y - that.y;
        return dx*dx + dy*dy;
    }
};

struct RectHV {
    double xmin;
    double ymin;
    double xmax;
    double ymax;

    RectHV(double xmin, double ymin, double xmax, double ymax)
        : xmin(xmin), ymin(ymin), xmax(xmax), ymax(ymax) {}

    bool contains(Point2D p) {
        return (p.x >= xmin) && (p.x <= xmax) && (p.y >= ymin) && (p.y <= ymax);
    }

    bool intersects(RectHV that) {
        return (this->xmax >= that.xmin) && (this->ymax >= that.ymin) &&
               (that.xmax >= this->xmin) && (that.ymax >= this->ymin);
    }

    double distanceSquaredTo(Point2D p) {
        double dx = 0.0, dy = 0.0;
        if      (p.x < xmin) dx = p.x - xmin;
        else if (p.x > xmax) dx = p.x - xmax;
        if      (p.y < ymin) dy = p.y - ymin;
        else if (p.y > ymax) dy = p.y - ymax;
        return dx*dx + dy*dy;
    }
};

class KdTree {
private:
    struct Node {
        Point2D p;
        RectHV rect;
        Node *leftBottom;
        Node *rightTop;

        Node(Point2D p, RectHV rect)
            : p(p), rect(rect), leftBottom(nullptr), rightTop(nullptr) {}
    };

    Node *root;
    int size;

public:
    KdTree() : root(nullptr), size(0) {}

    bool isEmpty() { return size == 0; }

    int size() { return size; }

    void insert(Point2D p) {
        root = insert(root, p, true, 0, 0, 1, 1);
    }

private:
    Node* insert(Node *x, Point2D p, bool vertical,
                 double xmin, double ymin, double xmax, double ymax) {
        if (x == nullptr) {
            size++;
            return new Node(p, RectHV(xmin, ymin, xmax, ymax));
        }
        if (x->p.x == p.x && x->p.y == p.y) return x;

        if (vertical) {
            if (p.x < x->p.x)
                x->leftBottom = insert(x->leftBottom, p,
                                       !vertical,
                                       xmin, ymin,
                                       x->p.x, ymax);
            else
                x->rightTop = insert(x->rightTop, p,
                                     !vertical,
                                     x->p.x, ymin,
                                     xmax, ymax);
        } else {
            if (p.y < x->p.y)
                x->leftBottom = insert(x->leftBottom ,p,
                                       !vertical,
                                       xmin ,ymin,
                                       xmax ,x->p.y);
            else
                x->rightTop = insert(x->rightTop ,p,
                                     !vertical,
                                     xmin ,x->p.y,
                                     xmax ,ymax);
        }
        return x;
    }

public:
    bool contains(Point2D p) {
        return contains(root ,p ,true);
    }

private:
    bool contains(Node *x ,Point2D p ,bool vertical){
       if(x==nullptr)return false;

       if(x->p.x==p.x&&x->p.y==p.y)return true;

       if(vertical){
           if(p.x<x->p.x)return contains(x->leftBottom ,p,!vertical);
           else return contains(x->rightTop,p,!vertical);
       }else{
           if(p.y<x->p.y)return contains(x->leftBottom,p,!vertical);
           else return contains(x->rightTop,p,!vertical);
       }
   }

public:
   std::vector<Point2D> range(RectHV rect){
       std::vector<Point2D> pointsInRange;
       range(root ,rect ,pointsInRange);
       return pointsInRange;
   }

private:
   void range(Node *node ,RectHV rect ,std::vector<Point2D> &pointsInRange){
       if(node==nullptr)return;

       if(rect.intersects(node->rect)){
           if(rect.contains(node->p))pointsInRange.push_back(node->p);

           range(node->leftBottom ,rect ,pointsInRange);
           range(node->rightTop ,rect ,pointsInRange);
       }
   }

public:
   Point2D nearest(Point2D p){
       return nearest(root,p,nullptr,true);
   }

private:
   Point2D nearest(Node *node ,Point2D point ,Point2D minPoint,bool vertical){
       Point2D min=minPoint;

       if(node==nullptr)return min;

       if(minPoint==nullptr||point.distanceSquaredTo(min)>point.distanceSquaredTo(node->p)){
           min=node->p;
       }

       Node *firstNode=nullptr;
       Node *secondNode=nullptr;

       if(vertical){
           if(point.x<node->p.x){
               firstNode=node->leftBottom;
               secondNode=node->rightTop;
           }else{
               firstNode=node->rightTop;
               secondNode=node->leftBottom;
           }
       }else{
           if(point.y<node->p.y){
               firstNode=node->leftBottom;
               secondNode=node->rightTop;
           }else{
               firstNode=node->rightTop;
               secondNode=node->leftBottom;
           }
       }

       min=nearest(firstNode ,point,min,!vertical);

       if(secondNode!=nullptr){
           RectHV rect=secondNode ->rect;

           Point2D point1(rect.xmin(),rect.ymin());
           Point2D point2(rect.xmax(),rect.ymax());

           double distance1=point.distanceSquaredTo(point1);
           double distance2=point.distanceSquaredTo(point2);

           double distance=std::min(distance1,distance2);

           if(distance<point.distanceSquaredTo(min)){
               min=nearest(secondNode ,point,min,!vertical);
           }
       }
       return min;
   }
};
 KdTree kdtree;

    kdtree.insert(Point2D(0.7, 0.2));
    kdtree.insert(Point2D(0.5, 0.4));
    kdtree.insert(Point2D(0.2, 0.3));
    kdtree.insert(Point2D(0.4, 0.7));
    kdtree.insert(Point2D(0.9, 0.6));

    std::cout << "Range search: " << std::endl;
    RectHV rect(0.3, 0.3, 0.6, 0.6);
    std::vector<Point2D> pointsInRange = kdtree.range(rect);
    for (Point2D p : pointsInRange) {
        std::cout << "(" << p.x << ", " << p.y << ")" << std::endl;
    }

    std::cout << "Nearest neighbor search: " << std::endl;
    Point2D queryPoint(0.8, 0.5);
    Point2D nearestPoint = kdtree.nearest(queryPoint);
    std::cout << "(" << nearestPoint.x << ", " << nearestPoint.y << ")" << std::endl;

    return 0;*/
