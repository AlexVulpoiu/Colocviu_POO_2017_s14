#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Participant
{
private:
    string nume, prenume, cnp;
    int zi, luna, an, varsta;

public:
    explicit Participant(string  n = "", string p = "", string c = "", const int& z = 0, const int& l = 0, const int& a = 0):
        nume{move(n)}, prenume{move(p)}, cnp{move(c)}, zi{z}, luna{l}, an{a}, varsta{2020 - a} {}

    friend istream& operator >>(istream& in, Participant& p);
    friend ostream& operator <<(ostream& out, const Participant& p)
    {
        out << p.nume << ' ' << p.prenume << '\n' << "CNP: " << p.cnp << '\n' << p.zi << '.' << p.luna << '.' << p.an << ", " << p.varsta << " ani\n";
        return out;
    }

    bool cnp_incorect();

    string get_nume() const
    {
        return nume;
    }
    string get_prenume() const
    {
        return prenume;
    }
    int get_varsta() const
    {
        return varsta;
    }
};

istream& operator >>(istream& in, Participant& p)
{
    cout << "Dati numele: ";
    in >> p.nume;
    cout << "\nDati prenumele: ";
    in >> p.prenume;
    cout << "\nDati CNP(13 cifre): ";
    in >> p.cnp;
    cout << "\nDati data nasterii(3 numere naturale, de forma zi luna an): ";
    in >> p.zi >> p.luna >> p.an;
    p.varsta = 2020 - p.an;

    return in;
}

bool Participant::cnp_incorect()
{
    int a, l, z;

    a = (int)cnp[1] * 10 + (int)cnp[2];
    l = (int)cnp[3] * 10 + (int)cnp[4];
    z = (int)cnp[5] * 10 + (int)cnp[6];

    return (a != an || l != luna || z != zi);
}


bool comp(const pair<Participant, int>& p1, const pair<Participant, int>& p2)
{
    if(p1.first.get_nume() < p2.first.get_nume())
        return true;
    return p1.first.get_nume() == p2.first.get_nume() && p1.first.get_prenume() < p2.first.get_prenume();
}

class Activitate
{
protected:
    string nume, data_inc;
    vector<pair<Participant, int>> participanti;

public:
    explicit Activitate(string n = "", string d = "", vector<pair<Participant, int>> p = {}): nume{move(n)}, data_inc{move(d)}, participanti{move(p)} {}
    virtual ~Activitate() = default;

    virtual void adauga_participant(const Participant& p, const int& x) = 0;
    void afiseaza_participanti_alfabetic();

    string get_nume() const
    {
        return nume;
    }
    string get_data() const
    {
        return data_inc;
    }
    vector<pair<Participant, int>> get_participanti() const
    {
        return participanti;
    }
};

void Activitate::afiseaza_participanti_alfabetic()
{
    cout << nume << ", " << participanti.size() << " participanti\n";
    sort(participanti.begin(), participanti.end(), comp);
    for(auto& it: participanti)
        cout << it.first << '\n';
}


class Curs: public Activitate
{
private:
    int durata, credite;

public:
    explicit Curs(const int& zile = 0, const int& cr = 0, const string& n = "", const string& d = "", const vector<pair<Participant, int>>& p = {}):
        durata{zile}, credite{cr}, Activitate(n, d, p) {}

    void adauga_participant(const Participant& p, const int& x) override
    {
        participanti.emplace_back(p, x);
    }
    void afiseaza_profesori()
    {
        cout << "Profesori participanti la " << nume << ":\n";
        sort(participanti.begin(), participanti.end(), comp);
        for(auto& it: participanti)
            if(it.first.get_varsta() > 18)
                cout << it.first << '\n';
    }
};


bool cmp(const pair<Participant, int>& p1, const pair<Participant, int>& p2)
{
    return p1.second > p2.second;
}

class Concurs: public Activitate
{
protected:
    vector<pair<Participant, int>> premianti;

public:
    explicit Concurs(const string& n = "", const string& d = "", const vector<pair<Participant, int>>& p = {}):
        Activitate(n, d, p) {}

    void adauga_participant(const Participant& p, const int& x) override;
    void afiseaza_premianti() const
    {
        cout << nume << ", " << premianti.size() << " premianti\n";
        for(auto& it: premianti)
            cout << "Punctaj: " << it.second << "\nDatele participantului: " << it.first << '\n';
    }
};

void Concurs::adauga_participant(const Participant &p, const int &x)
{
    int pct, nrp;

    participanti.emplace_back(p, x);

    premianti.emplace_back(p, x);
    sort(premianti.begin(), premianti.end(), cmp);
    pct = premianti.begin()->second;
    nrp = 1;
    for(auto& it: premianti)
        if(it.second != pct)
        {
            nrp++;
            pct = it.second;
        }
    while(nrp > 4)
    {
        while(premianti[premianti.size() - 1].second == pct)
        {
            pct = premianti[premianti.size() - 1].second;
            premianti.pop_back();
        }
        nrp--;
    }
}


class Concurs_Online: public Concurs
{
private:
    int durata;
    string site;

public:
    explicit Concurs_Online(const int& zile = 0, string s = "", const string& n = "", const string& d = "", const vector<pair<Participant, int>>& p = {}):
        durata{zile}, site{move(s)}, Concurs(n, d, p) {}
};


class Concurs_Offline: public Concurs
{
public:
    explicit Concurs_Offline(const string& n = "", const string& d = "", const vector<pair<Participant, int>>& p = {}):
        Concurs(n, d, p) {}
};


bool compara(const Participant& p1, const Participant& p2)
{
    return p1.get_nume() < p2.get_nume() || (p1.get_nume() == p2.get_nume() && p1.get_prenume() < p2.get_prenume());
}


int main()
{
    int comanda, n, i, poz, x;
    string nume, data;

    vector<Activitate*> act;
    act.push_back(new Curs(4, 10, "CP1", "30.05.2020"));
    act.push_back(new Curs(1, 5, "CP2", "27.05.2020"));
    act.push_back(new Curs(14, 20, "CP3", "01.06.2020"));
    act.push_back(new Concurs_Online(2, "www.ct1.com", "CT1", "25.05.2020"));
    act.push_back(new Concurs_Online(7, "www.ct2.com", "CT2", "28.01.2020"));
    act.push_back(new Concurs_Offline("CT3", "30.05.2020"));
    act.push_back(new Concurs_Offline("CT4", "01.06.2020"));

    while(true)
    {
        cout << "Alegeti optiunea:\n";
        cout << "0, pentru a opri executia\n";
        cout << "1, pentru a adauga n participanti la o activitate\n";
        cout << "2, pentru a afisa participantii alfabetic, grupati pe activitate\n";
        cout << "3, pentru a afisa alfabetic participantii la o activitate care incepe intr-o zi data\n";
        cout << "4, pentru a afisa premiantii concursurilor si profesorii participanti la cursuri\n";
        cin >> comanda;

        if(!comanda)
            break;
        else
            if(comanda == 1)
            {
                cout << "Dati numarul de participanti care urmeaza a fi adaugati: ";
                cin >> n;
                cout << '\n';
                while(n < 0)
                {
                    cout << "Numarul nu poate fi negativ. Introduceti din nou un numar natural: ";
                    cin >> n;
                    cout << '\n';
                }
                cout << "Dati numele activitatii din multimea {CP1, CP2, CP3, CT1, CT2, CT3, CT4}: ";
                cin >> nume;
                poz = -1;
                for(i = 0; i < act.size(); i++)
                    if(act[i]->get_nume() == nume)
                    {
                        poz = i;
                        break;
                    }
                cout << '\n';
                for(i = 0; i < n; i++)
                {
                    Participant p;
                    cin >> p;
                    while(p.cnp_incorect())
                    {
                        cout << "CNP-ul sau data nasterii au fost introduse gresit. introduceti din nou datele participantului:\n";
                        cin >> p;
                        cout << '\n';
                    }
                    if(nume[1] == 'P')
                        act[poz]->adauga_participant(p, 0);
                    else
                    {
                        while(p.get_varsta() > 18)
                        {
                            cout << "Concurentul trebuie sa aiba varsta <= 18 ani pentru a participa la concurs.\n";
                            cout << "Introduceti din nou datele participantului:\n";
                            cin >> p;
                            cout << '\n';
                        }
                        cout << "Dati punctajul concurentului(numar natural de la 0 la 100): ";
                        cin >> x;
                        cout << '\n';
                        act[poz]->adauga_participant(p, x);
                    }
                }
            }
            else
                if(comanda == 2)
                    for(auto& it: act)
                        it->afiseaza_participanti_alfabetic();
                else
                    if(comanda == 3)
                    {
                        vector<Participant> vp;
                        vector<pair<Participant, int>> vect;

                        cout << "Dati data dorita sub forma unui string cu structura: ZZ.LL.AAAA  ";
                        cin >> data;
                        vp.clear();
                        for(auto& it: act)
                            if(it->get_data() == data)
                            {
                                vect = it->get_participanti();
                                for(auto& itr: vect)
                                    vp.push_back(itr.first);
                            }
                        sort(vp.begin(), vp.end(), compara);
                    }
                    else
                        if(comanda == 4)
                        {
                            for(auto& it: act)
                                if(it->get_nume()[1] == 'T')
                                {
                                    Concurs aux = reinterpret_cast<const Concurs&>(it);
                                    aux.afiseaza_premianti();
                                }
                            for(auto& it: act)
                                if(it->get_nume()[1] == 'P')
                                {
                                    Curs aux = reinterpret_cast<const Curs&>(it);
                                    aux.afiseaza_profesori();
                                }
                        }
                        else
                            cout << "Comanda necunoscuta. Introduceti din nou comanda.\n";
    }

    return 0;
}
