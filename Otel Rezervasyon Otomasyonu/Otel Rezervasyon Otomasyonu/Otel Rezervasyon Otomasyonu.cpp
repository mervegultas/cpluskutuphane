#include <iostream>
#include <fstream> //Dosyalama işlemi
#include <string> //String manipülasyonu ve işlemleri
#include <string.h> //cin.get için
#include <chrono> //Zaman kütüphanesi
#include <thread> //Bekletmek için
#include <locale.h> //Türkçe dil desteği için
using namespace std;

static string musterikategori[7] = {
	"Kayıt No",
	"TC",
	"Ad",
	"Soyad",
	"Oda Numarası",
	"Giriş Tarihi",
	"Çıkış Tarihi"
};

static string personelkategori[6] = {
	"Kayıt No",
	"TC",
	"Ad",
	"Soyad",
	"Maaş",
	"Görev"
};

static string odakategori[4] = {
	"Oda Numarası(Otomatik atanması için -1 yazınız)",
	"Kat",
	"Yatak Sayısı",
	"Meşguliyet(Boş: 0, Dolu: 1)",
};

class insan
{
protected:
	int kayitNo;
	string TC;
	string ad;
	string soyad;
public:
	int getid() { return kayitNo; }
	string gettc() { return TC; }
	string getad() { return ad; }
	string getsoyad() { return soyad; }
	void setid(int id) { kayitNo = id; }
	void settc(string tcv) { TC = tcv; }
	void setad(string adv) { ad = adv; }
	void setsoyad(string soyadv) { soyad = soyadv; }
};

class musteri : public insan
{
protected:
	int odaNo;
	string girisTarih;
	string cikisTarih;
public:
	int getno() { return odaNo; }
	string getgt() { return girisTarih; }
	string getct() { return cikisTarih; }
	void setno(int no) { odaNo = no; }
	void setgt(string gt) { girisTarih = gt; }
	void setct(string ct) { cikisTarih = ct; }

	musteri() // Yapıcı fonksiyon, eklemeden önce kayıt no'yu dolduruyor
	{
		kayitNo = kayitNoBul() + 1;
	}

	~musteri() // Yıkıcı fonksiyon, değerleri sıfırlıyor.
	{
		kayitNo = -1;
		TC = "";
		ad = "";
		soyad = "";
		odaNo = -1;
		girisTarih = "";
		cikisTarih = "";
	}

	int kayitNoBul()
	{
		ifstream dosya;
		dosya.open("musteriler.txt", ios::in);
		if (dosya.is_open())
		{
			string satir;
			int sonid = 0;
			string id = "1";
			while (getline(dosya, satir))
			{
				id = "";
				int i = 0;
				while (satir[i] != '/')
				{
					id = id + satir[i];
					i++;
				}
				sonid = stoi(id); //STRING TO INT
			}
			dosya.close();
			return sonid;
		}
	}

	void musteriEkle()
	{
		kayitNo = kayitNoBul() + 1;
		string kayit = to_string(kayitNo) + "/" + TC + "/" + ad + "/" + soyad + "/" + to_string(odaNo) + "/" + girisTarih + "/" + cikisTarih + "/";

		ofstream dosya;
		dosya.open("musteriler.txt", ios::app);
		if (dosya.is_open())
		{
			dosya << kayit << "\n";
		}
		dosya.close();
	}

	void musteriSirala()
	{
		ifstream dosya;
		dosya.open("musteriler.txt", ios::in);
		if (dosya.is_open())
		{
			string satir;
			while (getline(dosya, satir))
			{
				cout << satir << "\n";
			}
		}
		dosya.close();
	}

	string musteriAra(string aranan, int kategori)
	{
		ifstream dosya;
		dosya.open("musteriler.txt", ios::in);
		if (dosya.is_open())
		{
			string satir;
			while (getline(dosya, satir))
			{
				int slashsayisi = kategori;
				int i = 0;
				string aralik = "";
				while (slashsayisi > -1)
				{
					if (satir[i] == '/')
					{
						slashsayisi--;
					}
					if (slashsayisi == 0)
					{
						if (satir[i] != '/')
						{
							aralik = aralik + satir[i];
						}
					}
					i++;
				}
				int arafark = aranan.compare(aralik);
				if (arafark == 0)
				{
					dosya.close();
					return satir;
				}
			}
		}
		dosya.close();
		return "Müşteri bulunamadı.";
	}

	void musteriGuncelle(int aramakategori, string guncellenecek, int guncellekategori, string veri)
	{
		fstream dosya;
		fstream temp;
		dosya.open("musteriler.txt", ios::in);
		temp.open("temp.txt", ios::app);

		if (dosya.is_open() && temp.is_open())
		{
			string satir;
			while (getline(dosya, satir)) //Dosyadan oku ve ara dongusu
			{
				int slashsayisi = aramakategori;
				int i = 0;
				string aralik = "";
				while (slashsayisi > -1) //Arama
				{
					if (satir[i] == '/')
					{
						slashsayisi--;
					}
					if (slashsayisi == 0)
					{
						if (satir[i] != '/')
						{
							aralik = aralik + satir[i];
						}
					}
					i++;
				}
				int arafark = guncellenecek.compare(aralik);
				if (arafark == 0) //guncellenecek veriyi class'a atmaya basla eger guncellenecek veri bulunduysa
				{
					slashsayisi = 0;
					aralik = "";

					for (i = 0; i < satir.length(); i++) // Butun satir bilgilerini class'a ata
					{
						if (satir[i] == '/')
						{
							if (slashsayisi == 0) { kayitNo = stoi(aralik); aralik = ""; }
							else if (slashsayisi == 1) { TC = aralik; aralik = ""; }
							else if (slashsayisi == 2) { ad = aralik; aralik = ""; }
							else if (slashsayisi == 3) { soyad = aralik; aralik = ""; }
							else if (slashsayisi == 4) { odaNo = stoi(aralik); aralik = ""; }
							else if (slashsayisi == 5) { girisTarih = aralik; aralik = ""; }
							else if (slashsayisi == 6) { cikisTarih = aralik; aralik = ""; }
							slashsayisi++;
						}
						else
						{
							aralik = aralik + satir[i];
						}
					}
					// Kategorimiz hangisiyse, o kategoriyi fonksiyona gelen veriye eşitliyoruz.
					if (guncellekategori == 0) { kayitNo = stoi(veri); }
					else if (guncellekategori == 1) { TC = veri; }
					else if (guncellekategori == 2) { ad = veri; }
					else if (guncellekategori == 3) { soyad = veri; }
					else if (guncellekategori == 4) { odaNo = stoi(veri); }
					else if (guncellekategori == 5) { girisTarih = veri; }
					else if (guncellekategori == 6) { cikisTarih = veri; }

					string kayit = to_string(kayitNo) + "/" + TC + "/" + ad + "/" + soyad + "/" + to_string(odaNo) + "/" + girisTarih + "/" + cikisTarih + "/";

					temp << kayit << "\n";
				}
				else
				{
					temp << satir << "\n";
				}
			}
		}
		dosya.close();
		temp.close();
		remove("musteriler.txt");

		dosya.open("musteriler.txt", ios::app);
		temp.open("temp.txt", ios::in);
		if (dosya.is_open() && temp.is_open())
		{
			string satir;
			while (getline(temp, satir)) //Dosyadan oku ve ara dongusu
			{
				dosya << satir << "\n";
			}
		}
		dosya.close();
		temp.close();
		remove("temp.txt");
	}

	void musteriSil(int silkategori, string silinecekveri)
	{
		fstream dosya;
		fstream temp;
		dosya.open("musteriler.txt", ios::in);
		temp.open("temp.txt", ios::app);

		if (dosya.is_open() && temp.is_open())
		{
			string satir;
			while (getline(dosya, satir)) //Dosyadan oku ve ara dongusu
			{
				int slashsayisi = silkategori;
				int i = 0;
				string aralik = "";
				while (slashsayisi > -1) //Arama
				{
					if (satir[i] == '/')
					{
						slashsayisi--;
					}
					if (slashsayisi == 0)
					{
						if (satir[i] != '/')
						{
							aralik = aralik + satir[i];
						}
					}
					i++;
				}
				int arafark = silinecekveri.compare(aralik);
				if (arafark != 0) //guncellenecek veriyi class'a atmaya basla eger guncellenecek veri bulunduysa
				{
					temp << satir << "\n";
				}
			}
		}
		dosya.close();
		temp.close();
		remove("musteriler.txt");

		dosya.open("musteriler.txt", ios::app);
		temp.open("temp.txt", ios::in);
		if (dosya.is_open() && temp.is_open())
		{
			string satir;
			while (getline(temp, satir)) //Dosyadan oku ve ara dongusu
			{
				dosya << satir << "\n";
			}
		}
		dosya.close();
		temp.close();
		remove("temp.txt");
	}
};

class personel : public insan
{
protected:
	int maas;
	string gorev;
public:
	int getmaas() { return maas; }
	string getgorev() { return gorev; }
	void setmaas(int maasv) { maas = maasv; }
	void setgorev(string gorevv) { gorev = gorevv; }

	personel() // Yapıcı fonksiyon, eklemeden önce kayıt no'yu dolduruyor
	{
		kayitNo = kayitNoBul() + 1;
	}

	~personel() // Yıkıcı fonksiyon, değerleri sıfırlıyor.
	{
		kayitNo = -1;
		TC = "";
		ad = "";
		soyad = "";
		maas = -1;
		gorev = "";
	}

	int kayitNoBul()
	{
		ifstream dosya;
		dosya.open("personel.txt", ios::in);
		if (dosya.is_open())
		{
			string satir;
			int sonid = 0;
			string id = "1";
			while (getline(dosya, satir))
			{
				id = "";
				int i = 0;
				while (satir[i] != '/')
				{
					id = id + satir[i];
					i++;
				}
				sonid = stoi(id); //STRING TO INT
			}
			dosya.close();
			return sonid;
		}
	}

	void personelEkle()
	{
		kayitNo = kayitNoBul() + 1;
		string kayit = to_string(kayitNo) + "/" + TC + "/" + ad + "/" + soyad + "/" + to_string(maas) + "/" + gorev + "/";

		ofstream dosya;
		dosya.open("personel.txt", ios::app);
		if (dosya.is_open())
		{
			dosya << kayit << "\n";
		}
		dosya.close();
	}

	void personelSirala()
	{
		ifstream dosya;
		dosya.open("personel.txt", ios::in);
		if (dosya.is_open())
		{
			string satir;
			while (getline(dosya, satir))
			{
				cout << satir << "\n";
			}
		}
		dosya.close();
	}

	string personelAra(string aranan, int kategori)
	{
		ifstream dosya;
		dosya.open("personel.txt", ios::in);
		if (dosya.is_open())
		{
			string satir;
			while (getline(dosya, satir))
			{
				int slashsayisi = kategori;
				int i = 0;
				string aralik = "";
				while (slashsayisi > -1)
				{
					if (satir[i] == '/')
					{
						slashsayisi--;
					}
					if (slashsayisi == 0)
					{
						if (satir[i] != '/')
						{
							aralik = aralik + satir[i];
						}
					}
					i++;
				}
				int arafark = aranan.compare(aralik);
				if (arafark == 0)
				{
					dosya.close();
					return satir;
				}
			}
		}
		dosya.close();
		return "Personel bulunamadı.";
	}

	void personelGuncelle(int aramakategori, string guncellenecek, int guncellekategori, string veri)
	{
		fstream dosya;
		fstream temp;
		dosya.open("personel.txt", ios::in);
		temp.open("temp.txt", ios::app);

		if (dosya.is_open() && temp.is_open())
		{
			string satir;
			while (getline(dosya, satir)) //Dosyadan oku ve ara dongusu
			{
				int slashsayisi = aramakategori;
				int i = 0;
				string aralik = "";
				while (slashsayisi > -1) //Arama
				{
					if (satir[i] == '/')
					{
						slashsayisi--;
					}
					if (slashsayisi == 0)
					{
						if (satir[i] != '/')
						{
							aralik = aralik + satir[i];
						}
					}
					i++;
				}
				int arafark = guncellenecek.compare(aralik);
				if (arafark == 0) //guncellenecek veriyi class'a atmaya basla eger guncellenecek veri bulunduysa
				{
					slashsayisi = 0;
					aralik = "";

					for (i = 0; i < satir.length(); i++) // Butun satir bilgilerini class'a ata
					{
						if (satir[i] == '/')
						{
							if (slashsayisi == 0) { kayitNo = stoi(aralik); aralik = ""; }
							else if (slashsayisi == 1) { TC = aralik; aralik = ""; }
							else if (slashsayisi == 2) { ad = aralik; aralik = ""; }
							else if (slashsayisi == 3) { soyad = aralik; aralik = ""; }
							else if (slashsayisi == 4) { maas = stoi(aralik); aralik = ""; }
							else if (slashsayisi == 5) { gorev = aralik; aralik = ""; }
							slashsayisi++;
						}
						else
						{
							aralik = aralik + satir[i];
						}
					}
					// Kategorimiz hangisiyse, o kategoriyi fonksiyona gelen veriye eşitliyoruz.
					if (guncellekategori == 0) { kayitNo = stoi(veri); }
					else if (guncellekategori == 1) { TC = veri; }
					else if (guncellekategori == 2) { ad = veri; }
					else if (guncellekategori == 3) { soyad = veri; }
					else if (guncellekategori == 4) { maas = stoi(veri); }
					else if (guncellekategori == 5) { gorev = veri; }

					string kayit = to_string(kayitNo) + "/" + TC + "/" + ad + "/" + soyad + "/" + to_string(maas) + "/" + gorev + "/";

					temp << kayit << "\n";
				}
				else
				{
					temp << satir << "\n";
				}
			}
		}
		dosya.close();
		temp.close();
		remove("personel.txt");

		dosya.open("personel.txt", ios::app);
		temp.open("temp.txt", ios::in);
		if (dosya.is_open() && temp.is_open())
		{
			string satir;
			while (getline(temp, satir)) //Dosyadan oku ve ara dongusu
			{
				dosya << satir << "\n";
			}
		}
		dosya.close();
		temp.close();
		remove("temp.txt");
	}

	void personelSil(int silkategori, string silinecekveri)
	{
		fstream dosya;
		fstream temp;
		dosya.open("personel.txt", ios::in);
		temp.open("temp.txt", ios::app);

		if (dosya.is_open() && temp.is_open())
		{
			string satir;
			while (getline(dosya, satir)) //Dosyadan oku ve ara dongusu
			{
				int slashsayisi = silkategori;
				int i = 0;
				string aralik = "";
				while (slashsayisi > -1) //Arama
				{
					if (satir[i] == '/')
					{
						slashsayisi--;
					}
					if (slashsayisi == 0)
					{
						if (satir[i] != '/')
						{
							aralik = aralik + satir[i];
						}
					}
					i++;
				}
				int arafark = silinecekveri.compare(aralik);
				if (arafark != 0) //guncellenecek veriyi class'a atmaya basla eger guncellenecek veri bulunduysa
				{
					temp << satir << "\n";
				}
			}
		}
		dosya.close();
		temp.close();
		remove("personel.txt");

		dosya.open("personel.txt", ios::app);
		temp.open("temp.txt", ios::in);
		if (dosya.is_open() && temp.is_open())
		{
			string satir;
			while (getline(temp, satir)) //Dosyadan oku ve ara dongusu
			{
				dosya << satir << "\n";
			}
		}
		dosya.close();
		temp.close();
		remove("temp.txt");
	}
};

class oda
{
protected:
	int odaNo;
	int kat;
	int yatakSayisi;
	bool mesguliyet;
public:
	int getoda() { return odaNo; }
	int getkat() { return kat; }
	int getys() { return yatakSayisi; }
	bool getmes() { return mesguliyet; }
	void setoda(int odav) { odaNo = odav; }
	void setkat(int katv) { kat = katv; }
	void setys(int ys) { yatakSayisi = ys; }
	void setmes(bool mes) { mesguliyet = mes; }

	oda() // Yapıcı fonksiyon, eklemeden önce kayıt no'yu dolduruyor
	{
		odaNo = odaNoBul() + 1;
	}

	~oda() // Yıkıcı fonksiyon, değerleri sıfırlıyor.
	{
		odaNo = -1;
		kat = -1;
		yatakSayisi = -1;
		mesguliyet = false;
	}

	int odaNoBul()
	{
		ifstream dosya;
		dosya.open("oda.txt", ios::in);
		if (dosya.is_open())
		{
			string satir;
			int sonid = 0;
			string id = "1";
			while (getline(dosya, satir))
			{
				id = "";
				int i = 0;
				while (satir[i] != '/')
				{
					id = id + satir[i];
					i++;
				}
				sonid = stoi(id); //STRING TO INT
			}
			dosya.close();
			return sonid;
		}
	}

	void odaEkle(bool odaNoGirildi)
	{
		if (!odaNoGirildi)
		{
			odaNo = odaNoBul() + 1;
		}
		string kayit = to_string(odaNo) + "/" + to_string(kat) + "/" + to_string(yatakSayisi) + "/" + to_string(mesguliyet) + "/";

		ofstream dosya;
		dosya.open("oda.txt", ios::app);
		if (dosya.is_open())
		{
			dosya << kayit << "\n";
		}
		dosya.close();
	}

	void odaSirala()
	{
		ifstream dosya;
		dosya.open("oda.txt", ios::in);
		if (dosya.is_open())
		{
			string satir;
			while (getline(dosya, satir))
			{
				cout << satir << "\n";
			}
		}
		dosya.close();
	}

	string odaAra(string aranan, int kategori)
	{
		ifstream dosya;
		dosya.open("oda.txt", ios::in);
		if (dosya.is_open())
		{
			string satir;
			while (getline(dosya, satir))
			{
				int slashsayisi = kategori;
				int i = 0;
				string aralik = "";
				while (slashsayisi > -1)
				{
					if (satir[i] == '/')
					{
						slashsayisi--;
					}
					if (slashsayisi == 0)
					{
						if (satir[i] != '/')
						{
							aralik = aralik + satir[i];
						}
					}
					i++;
				}
				int arafark = aranan.compare(aralik);
				if (arafark == 0)
				{
					dosya.close();
					return satir;
				}
			}
		}
		dosya.close();
		return "Oda bulunamadı.";
	}

	void odaGuncelle(int aramakategori, string guncellenecek, int guncellekategori, string veri)
	{
		fstream dosya;
		fstream temp;
		dosya.open("oda.txt", ios::in);
		temp.open("temp.txt", ios::app);

		if (dosya.is_open() && temp.is_open())
		{
			string satir;
			while (getline(dosya, satir)) //Dosyadan oku ve ara dongusu
			{
				int slashsayisi = aramakategori;
				int i = 0;
				string aralik = "";
				while (slashsayisi > -1) //Arama
				{
					if (satir[i] == '/')
					{
						slashsayisi--;
					}
					if (slashsayisi == 0)
					{
						if (satir[i] != '/')
						{
							aralik = aralik + satir[i];
						}
					}
					i++;
				}
				int arafark = guncellenecek.compare(aralik);
				if (arafark == 0) //guncellenecek veriyi class'a atmaya basla eger guncellenecek veri bulunduysa
				{
					slashsayisi = 0;
					aralik = "";

					for (i = 0; i < satir.length(); i++) // Butun satir bilgilerini class'a ata
					{
						if (satir[i] == '/')
						{
							if (slashsayisi == 0) { odaNo = stoi(aralik); aralik = ""; }
							else if (slashsayisi == 1) { kat = stoi(aralik); aralik = ""; }
							else if (slashsayisi == 2) { yatakSayisi = stoi(aralik); aralik = ""; }
							else if (slashsayisi == 3) { mesguliyet = stoi(aralik); aralik = ""; }
							slashsayisi++;
						}
						else
						{
							aralik = aralik + satir[i];
						}
					}
					// Kategorimiz hangisiyse, o kategoriyi fonksiyona gelen veriye eşitliyoruz.
					if (guncellekategori == 0) { odaNo = stoi(veri); }
					else if (guncellekategori == 1) { kat = stoi(veri); }
					else if (guncellekategori == 2) { yatakSayisi = stoi(veri); }
					else if (guncellekategori == 3) { mesguliyet = stoi(veri); }

					string kayit = to_string(odaNo) + "/" + to_string(kat) + "/" + to_string(yatakSayisi) + "/" + to_string(mesguliyet) + "/";

					temp << kayit << "\n";
				}
				else
				{
					temp << satir << "\n";
				}
			}
		}
		dosya.close();
		temp.close();
		remove("oda.txt");

		dosya.open("oda.txt", ios::app);
		temp.open("temp.txt", ios::in);
		if (dosya.is_open() && temp.is_open())
		{
			string satir;
			while (getline(temp, satir)) //Dosyadan oku ve ara dongusu
			{
				dosya << satir << "\n";
			}
		}
		dosya.close();
		temp.close();
		remove("temp.txt");
	}

	void odaSil(int silkategori, string silinecekveri)
	{
		fstream dosya;
		fstream temp;
		dosya.open("oda.txt", ios::in);
		temp.open("temp.txt", ios::app);

		if (dosya.is_open() && temp.is_open())
		{
			string satir;
			while (getline(dosya, satir)) //Dosyadan oku ve ara dongusu
			{
				int slashsayisi = silkategori;
				int i = 0;
				string aralik = "";
				while (slashsayisi > -1) //Arama
				{
					if (satir[i] == '/')
					{
						slashsayisi--;
					}
					if (slashsayisi == 0)
					{
						if (satir[i] != '/')
						{
							aralik = aralik + satir[i];
						}
					}
					i++;
				}
				int arafark = silinecekveri.compare(aralik);
				if (arafark != 0) //guncellenecek veriyi class'a atmaya basla eger guncellenecek veri bulunduysa
				{
					temp << satir << "\n";
				}
			}
		}
		dosya.close();
		temp.close();
		remove("oda.txt");

		dosya.open("oda.txt", ios::app);
		temp.open("temp.txt", ios::in);
		if (dosya.is_open() && temp.is_open())
		{
			string satir;
			while (getline(temp, satir)) //Dosyadan oku ve ara dongusu
			{
				dosya << satir << "\n";
			}
		}
		dosya.close();
		temp.close();
		remove("temp.txt");
	}
};

void yavasyaz(string mesaj, char sart, int uzun, int kisa)
{
	using namespace this_thread;
	using namespace chrono;
	for (int i = 0; i < mesaj.length(); i++)
	{
		cout << mesaj[i];
		if (mesaj[i] == sart)
		{
			sleep_for(milliseconds(uzun));
		}
		else
		{
			sleep_for(milliseconds(kisa));
		}
	}
}

int main()
{
	setlocale(LC_ALL, "Turkish"); //Türkçe karakterler için
	//Giriş panelimsi havalı görüntü
	string girisekrani = "Lütfen devam etmek için giriş yapınız;\nKullanıcı Adı: mervegultas\nŞifre: **********\n\n:Giriş yapılıyor:";
	yavasyaz(girisekrani, ':', 100, 5);
	system("cls"); //Ekran temizleme

	int islem1, islem2;
	char islem3;
	do {
		string menuyazisi = "İşlemler:\n1. Müşteriler;\n2. Personeller;\n3. Odalar;\n4. Çıkış;\nYapmak istediğiniz işlemi giriniz: ";
		yavasyaz(menuyazisi, ';', 100, 5);
		cin >> islem1;
		system("cls"); //Ekran temizleme
		string bulunan;

		switch (islem1)
		{
		case 1: //Müşteriler
			do {
				menuyazisi = "İşlemler:\n1. Müşteri ekle\n2. Müşteri listele\n3. Müşteri arat\n4. Müşteri güncelle\n5. Müşteri sil\n6. Çıkış\nYapmak istediğiniz işlemi giriniz: ";
				yavasyaz(menuyazisi, '\n', 200, 10);
				cin >> islem2;
				system("cls"); //Ekran temizleme

				musteri musteriislem;
				int kategori, kategori2;
				char veri[30], veri2[30];
				switch (islem2)
				{
				case 1:
					for (int i = 1; i < 7; i++)
					{
						cout << musterikategori[i] << ": ";
						cin.ignore(256, '\n');
						cin.get(veri, 30);
						if (i == 1) { musteriislem.settc(veri); }
						else if (i == 2) { musteriislem.setad(veri); }
						else if (i == 3) { musteriislem.setsoyad(veri); }
						else if (i == 4) { musteriislem.setno(stoi(veri)); }
						else if (i == 5) { musteriislem.setgt(veri); }
						else if (i == 6) { musteriislem.setct(veri); }
					}
					musteriislem.musteriEkle();
					cout << "Müşteri başarıyla eklendi!\nDevam etmek için bir tuşa basınız...\n\n";
					system("pause"); //Ekran bekletme
					system("cls"); //Ekran temizleme
					break;
				case 2:
					cout << "Kayıt No / TC / Ad / Soyad / Oda No / Giriş Tarihi / Çıkış Tarihi\n";
					musteriislem.musteriSirala();
					cout << "\nDevam etmek için bir tuşa basınız.\n\n";
					system("pause"); //Ekran bekletme
					system("cls"); //Ekran temizleme
					break;
				case 3:
					for (int i = 0; i < 7; i++)
					{
						cout << i << ": " << musterikategori[i] << endl;
					}
					cout << "Kategori seçiniz: ";
					cin >> kategori;
					cout << "Aramak istediğiniz veriyi giriniz: ";
					cin.ignore(256, '\n');
					cin.get(veri, 30);
					cout << musteriislem.musteriAra(veri, kategori);

					cout << "\nDevam etmek için bir tuşa basınız.\n\n";
					system("pause"); //Ekran bekletme
					system("cls"); //Ekran temizleme
					break;
				case 4:
					for (int i = 0; i < 7; i++)
					{
						cout << i << ": " << musterikategori[i] << endl;
					}
					cout << "Kategori seçiniz: ";
					cin >> kategori;
					cout << "Güncellemek istediğiniz satırın verisini giriniz: ";
					cin.ignore(256, '\n');
					cin.get(veri, 30);
					bulunan = musteriislem.musteriAra(veri, kategori);
					cout << bulunan;

					if (bulunan != "Müşteri bulunamadı.")
					{
						cout << "\nBu satırı güncellemek istediğinize emin misiniz? (E/H)";
						cin >> islem3;
						if (islem3 == 'E' || islem3 == 'e')
						{
							cout << endl;
							for (int i = 1; i < 7; i++)
							{
								cout << i << ": " << musterikategori[i] << endl;
							}
							cout << "Değiştirmek istediğiniz kategoriyi seçiniz: ";
							cin >> kategori2;
							cout << "Yeni veriyi giriniz: ";
							cin.ignore(256, '\n');
							cin.get(veri2, 30);
							musteriislem.musteriGuncelle(kategori, veri, kategori2, veri2);

							cout << "Müşteri başarıyla güncellendi!\nDevam etmek için bir tuşa basınız...\n\n";
						}
					}
					system("pause"); //Ekran bekletme
					system("cls"); //Ekran temizleme
					break;
				case 5:
					for (int i = 0; i < 7; i++)
					{
						cout << i << ": " << musterikategori[i] << endl;
					}
					cout << "Kategori seçiniz: ";
					cin >> kategori;
					cout << "Silmek istediğiniz satırın verisini giriniz: ";
					cin.ignore(256, '\n');
					cin.get(veri, 30);
					bulunan = musteriislem.musteriAra(veri, kategori);
					cout << bulunan;

					if (bulunan != "Müşteri bulunamadı.")
					{
						cout << "\nBu satırı silmek istediğinize emin misiniz? (E/H)";
						cin >> islem3;
						if (islem3 == 'E' || islem3 == 'e')
						{
							musteriislem.musteriSil(kategori, veri);

							cout << "Müşteri başarıyla silindi!\nDevam etmek için bir tuşa basınız...\n\n";
						}
					}
					system("pause"); //Ekran bekletme
					system("cls"); //Ekran temizleme
					break;
				}
			} while (islem2 != 6);
			break;

		case 2: //Personeller
			do {
				menuyazisi = "İşlemler:\n1. Personel ekle\n2. Personel listele\n3. Personel arat\n4. Personel güncelle\n5. Personel sil\n6. Çıkış\nYapmak istediğiniz işlemi giriniz: ";
				yavasyaz(menuyazisi, '\n', 200, 10);
				cin >> islem2;
				system("cls"); //Ekran temizleme

				personel personelislem;
				int kategori, kategori2;
				char veri[30], veri2[30];
				switch (islem2)
				{
				case 1:
					for (int i = 1; i < 6; i++)
					{
						cout << personelkategori[i] << ": ";
						cin.ignore(256, '\n');
						cin.get(veri, 30);
						if (i == 1) { personelislem.settc(veri); }
						else if (i == 2) { personelislem.setad(veri); }
						else if (i == 3) { personelislem.setsoyad(veri); }
						else if (i == 4) { personelislem.setmaas(stoi(veri)); }
						else if (i == 5) { personelislem.setgorev(veri); }
					}
					personelislem.personelEkle();
					cout << "Personel başarıyla eklendi!\nDevam etmek için bir tuşa basınız...\n\n";
					system("pause"); //Ekran bekletme
					system("cls"); //Ekran temizleme
					break;
				case 2:
					cout << "Kayıt No / TC / Ad / Soyad / Maaş / Görev\n";
					personelislem.personelSirala();
					cout << "\nDevam etmek için bir tuşa basınız.\n\n";
					system("pause"); //Ekran bekletme
					system("cls"); //Ekran temizleme
					break;
				case 3:
					for (int i = 0; i < 6; i++)
					{
						cout << i << ": " << personelkategori[i] << endl;
					}
					cout << "Kategori seçiniz: ";
					cin >> kategori;
					cout << "Aramak istediğiniz veriyi giriniz: ";
					cin.ignore(256, '\n');
					cin.get(veri, 30);
					cout << personelislem.personelAra(veri, kategori);

					cout << "\nDevam etmek için bir tuşa basınız.\n\n";
					system("pause"); //Ekran bekletme
					system("cls"); //Ekran temizleme
					break;
				case 4:
					for (int i = 0; i < 6; i++)
					{
						cout << i << ": " << personelkategori[i] << endl;
					}
					cout << "Kategori seçiniz: ";
					cin >> kategori;
					cout << "Güncellemek istediğiniz satırın verisini giriniz: ";
					cin.ignore(256, '\n');
					cin.get(veri, 30);
					bulunan = personelislem.personelAra(veri, kategori);
					cout << bulunan;

					if (bulunan != "Personel bulunamadı.")
					{
						cout << "\nBu satırı güncellemek istediğinize emin misiniz? (E/H)";
						cin >> islem3;
						if (islem3 == 'E' || islem3 == 'e')
						{
							cout << endl;
							for (int i = 1; i < 6; i++)
							{
								cout << i << ": " << personelkategori[i] << endl;
							}
							cout << "Değiştirmek istediğiniz kategoriyi seçiniz: ";
							cin >> kategori2;
							cout << "Yeni veriyi giriniz: ";
							cin.ignore(256, '\n');
							cin.get(veri2, 30);
							personelislem.personelGuncelle(kategori, veri, kategori2, veri2);

							cout << "Personel başarıyla güncellendi!\nDevam etmek için bir tuşa basınız...\n\n";
						}
					}
					system("pause"); //Ekran bekletme
					system("cls"); //Ekran temizleme
					break;
				case 5:
					for (int i = 0; i < 6; i++)
					{
						cout << i << ": " << personelkategori[i] << endl;
					}
					cout << "Kategori seçiniz: ";
					cin >> kategori;
					cout << "Silmek istediğiniz satırın verisini giriniz: ";
					cin.ignore(256, '\n');
					cin.get(veri, 30);
					bulunan = personelislem.personelAra(veri, kategori);
					cout << bulunan;

					if (bulunan != "Personel bulunamadı.")
					{
						cout << "\nBu satırı silmek istediğinize emin misiniz? (E/H)";
						cin >> islem3;
						if (islem3 == 'E' || islem3 == 'e')
						{
							personelislem.personelSil(kategori, veri);

							cout << "Personel başarıyla silindi!\nDevam etmek için bir tuşa basınız...\n\n";
						}
					}
					system("pause"); //Ekran bekletme
					system("cls"); //Ekran temizleme
					break;
				}
			} while (islem2 != 6);
			break;
		case 3: //Odalar
			do {
				menuyazisi = "İşlemler:\n1. Oda ekle\n2. Oda listele\n3. Oda arat\n4. Oda güncelle\n5. Oda sil\n6. Çıkış\nYapmak istediğiniz işlemi giriniz: ";
				yavasyaz(menuyazisi, '\n', 200, 10);
				cin >> islem2;
				system("cls"); //Ekran temizleme

				oda odaislem;
				int kategori, kategori2;
				char veri[30], veri2[30];
				bool odanogirildi = false;

				switch (islem2)
				{
				case 1:
					for (int i = 0; i < 4; i++)
					{
						cout << odakategori[i] << ": ";
						cin.ignore(256, '\n');
						cin.get(veri, 30);
						if (i == 0)
						{
							if (stoi(veri) != -1)
							{
								odaislem.setoda(stoi(veri));
								odanogirildi = true;
							}
						}
						else if (i == 1) { odaislem.setkat(stoi(veri)); }
						else if (i == 2) { odaislem.setys(stoi(veri)); }
						else if (i == 3) { odaislem.setmes(stoi(veri)); }
					}
					odaislem.odaEkle(odanogirildi);
					cout << "Oda başarıyla eklendi!\nDevam etmek için bir tuşa basınız...\n\n";
					system("pause"); //Ekran bekletme
					system("cls"); //Ekran temizleme
					break;
				case 2:
					cout << "Oda No / Kat / Yatak Sayısı / Meşguliyet\n";
					odaislem.odaSirala();
					cout << "\nDevam etmek için bir tuşa basınız.\n\n";
					system("pause"); //Ekran bekletme
					system("cls"); //Ekran temizleme
					break;
				case 3:
					for (int i = 0; i < 4; i++)
					{
						cout << i << ": " << odakategori[i] << endl;
					}
					cout << "Kategori seçiniz: ";
					cin >> kategori;
					cout << "Aramak istediğiniz veriyi giriniz: ";
					cin.ignore(256, '\n');
					cin.get(veri, 30);
					cout << odaislem.odaAra(veri, kategori);

					cout << "\nDevam etmek için bir tuşa basınız.\n\n";
					system("pause"); //Ekran bekletme
					system("cls"); //Ekran temizleme
					break;
				case 4:
					for (int i = 0; i < 4; i++)
					{
						cout << i << ": " << odakategori[i] << endl;
					}
					cout << "Kategori seçiniz: ";
					cin >> kategori;
					cout << "Güncellemek istediğiniz satırın verisini giriniz: ";
					cin.ignore(256, '\n');
					cin.get(veri, 30);
					bulunan = odaislem.odaAra(veri, kategori);
					cout << bulunan;

					if (bulunan != "Oda bulunamadı.")
					{
						cout << "\nBu satırı güncellemek istediğinize emin misiniz? (E/H): ";
						cin >> islem3;
						if (islem3 == 'E' || islem3 == 'e')
						{
							cout << endl;
							for (int i = 1; i < 4; i++)
							{
								cout << i << ": " << odakategori[i] << endl;
							}
							cout << "Değiştirmek istediğiniz kategoriyi seçiniz: ";
							cin >> kategori2;
							cout << "Yeni veriyi giriniz: ";
							cin.ignore(256, '\n');
							cin.get(veri2, 30);
							odaislem.odaGuncelle(kategori, veri, kategori2, veri2);

							cout << "Oda başarıyla güncellendi!\nDevam etmek için bir tuşa basınız...\n\n";
						}
					}
					system("pause"); //Ekran bekletme
					system("cls"); //Ekran temizleme
					break;
				case 5:
					for (int i = 0; i < 4; i++)
					{
						cout << i << ": " << odakategori[i] << endl;
					}
					cout << "Kategori seçiniz: ";
					cin >> kategori;
					cout << "Silmek istediğiniz satırın verisini giriniz: ";
					cin.ignore(256, '\n');
					cin.get(veri, 30);
					bulunan = odaislem.odaAra(veri, kategori);
					cout << bulunan;

					if (bulunan != "Oda bulunamadı.")
					{
						cout << "\nBu satırı silmek istediğinize emin misiniz? (E/H)";
						cin >> islem3;
						if (islem3 == 'E' || islem3 == 'e')
						{
							odaislem.odaSil(kategori, veri);

							cout << "Oda başarıyla silindi!\nDevam etmek için bir tuşa basınız...\n\n";
						}
					}
					system("pause"); //Ekran bekletme
					system("cls"); //Ekran temizleme
					break;
				}
			} while (islem2 != 6);
			break;
		}
	} while (islem1 != 4);
}