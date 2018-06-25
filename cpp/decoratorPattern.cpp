#include <cstdio>
#include <memory>

 class  tComponent{
 public:
     virtual void printTicket() = 0;
 };


class tSalesTicket: public tComponent{
public:
    tSalesTicket(){

    }

    void printTicket() override{
        printf("this is the ticket \n");
    }
};

class tDecorator: public tComponent{
public:

    tDecorator(std::unique_ptr<tComponent> xComponent):m_xComponent(std::move(xComponent))
    {

    }

    void printTicket() override
    {
        if(m_xComponent){
            m_xComponent->printTicket();
        }
    }
private:
    std::unique_ptr<tComponent> m_xComponent;
};


class tHeader1 : public tDecorator{
public:
    tHeader1(std::unique_ptr<tComponent> xComponent):
        tDecorator(std::move(xComponent)) {

    }

    void printTicket() override{
        printf("this is header \n");

        tDecorator::printTicket();
    }
};

class tFooter1: public tDecorator{
public:
    tFooter1(std::unique_ptr<tComponent> xComponent):
        tDecorator(std::move(xComponent)) {

    }

    void printTicket() override{
        tDecorator::printTicket();
        printf("this is footer1 \n");
    }

};



int main(){
    auto xComponent = std::make_unique<tHeader1>(std::make_unique<tFooter1>(std::make_unique<tSalesTicket>()));   //memory leak
    xComponent->printTicket();

    return 0;
}
