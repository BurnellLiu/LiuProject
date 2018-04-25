
#include <cstdio>

#include "LSignalSlot.h"


class Button
{
public:
    LSignal0 ClickSignal;

    void NowClick(bool b)
    {
        this->ClickSignal.Emit();
    }
};

class Tv : public LSlot
{
public:
    Tv(int i) 
    {
        m_id = i;
    }

    void OnButtonClicked()
    {
        
        printf("Tv %d is being turned on\n", m_id);
    }

private:
    int m_id;
};


int main()
{
    Button btn;

    Tv* tv1 = new Tv(1);
    Tv* tv2 = new Tv(2);

    btn.ClickSignal.Connect(tv1, &Tv::OnButtonClicked);
    btn.ClickSignal.Connect(tv2, &Tv::OnButtonClicked);

    //btn.ClickSignal.Disconnect(tv1);
    // delete tv1;
    btn.NowClick(true);

    system("pause");

    return 0;
}