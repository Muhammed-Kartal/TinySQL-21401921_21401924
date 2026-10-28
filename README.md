# TinySQL – Mini Veritabanı Motoru

INF224 Proje Ödevi. Hash tablosu, AVL ağacı ve dizi tabanlı heap dosyası kullanan tek tablolu bir veritabanı motoru.

## Derleme ve çalıştırma

- `make` → derler, `tinysql` programını oluşturur
- `./tinysql` → REPL'i başlatır
- `./tinysql --echo` → okunan her komutu ekrana da yazar (komutlar dosyadan verildiğinde kullanılır)
- `make data` → `data/students.csv` dosyasını üretir (10.000 kayıt, sabit tohum, her platformda aynı dosya)
- `make bench` → ölçüm programını çalıştırır, sonuçları `report/olcumler.md` dosyasına yazar
- `make demo` → `data/demo.sql` komutlarını REPL'de çalıştırır, çıktıyı `report/repl_cikti.txt` dosyasına yazar
- `make debug` → bellek hatası kontrolüyle (`-fsanitize=address`) derler
- `make clean` → derleme çıktılarını siler

Bellek sızıntısı kontrolü:
- Linux: `valgrind ./tinysql`
- macOS: `printf 'LOAD data/students.csv\nEXIT\n' | leaks --atExit -- ./tinysql` (normal `make` derlemesiyle; `make debug` ile çalışmaz). macOS'ta `-fsanitize=address` sızıntı denetlemez, sadece taşma ve geçersiz erişim yakalar.

## Dosya yapısı

- `src/tinysql.h` → `Record` yapısı ve ortak sabitler
- `src/heap.*` → Adım 1: dizi tabanlı depolama
- `src/hash_index.*` → Adım 2: id üzerinde hash indeksi
- `src/avl_index.*` → Adım 3: GPA üzerinde AVL indeksi
- `src/table.*` → üç yapıyı tutarlı tutan entegrasyon katmanı
- `src/query.*` → Adım 4: ayrıştırıcı, sorgu yürütücü, REPL
- `src/main.c` → giriş noktası
- `data/students.csv` → 10.000 kayıtlık veri
- `data/demo.sql` → test senaryosu madde 10–11 için REPL komutları
- `tools/gen_csv.c` → CSV üreticisi
- `tools/bench.c` → ölçüm programı (test senaryosu madde 1–9 ve 12)
- `report/olcumler.md` → `make bench` çıktısı (raporun tabloları)
- `report/repl_cikti.txt` → `make demo` çıktısı (REPL kanıtı)

`tools/` klasörü ödevin dosya yapısında yoktur. Kendi `main` fonksiyonu olan yardımcı programlar `src/` içine konulursa `tinysql` ile çakışacağı için ayrı tutulmuştur.

## Sözleşme kararları

### Genel
- Table dışındaki hiçbir kod heap, hash veya AVL'ye doğrudan yazmaz. Tüm ekleme ve silme işlemleri Table fonksiyonlarından geçer. (Ölçüm programı istisnadır: sadece okur.)
- Table yaklaşık 10 MB olduğu için `table_create` ile `malloc` üzerinden ayrılır, stack'te tutulmaz.
- Stub'lar başarısızlık değeri döndürür (–1 veya NULL). Yazılmamış bir fonksiyon asla başarılı görünmemelidir.
- Bir dosya neye ihtiyaç duyuyorsa onu kendisi include eder. Standart başlıklar `.h` dosyalarına değil, ihtiyaç duyan `.c` dosyalarına eklenir.
- Kullanıcıya görünen tüm çıktılar (printf) Türkçe yazılır. Kod içindeki isimler (fonksiyon, değişken, struct) İngilizcedir.
- Big-O yorum satırları kod tamamlandıktan sonra, en son eklenecek. Değerler ödevdeki tablolardan alınır.
- `sscanf` biçimlerinde metin alanları sınırlıdır: isim için `%63`, bölüm için `%31` (`MAX_NAME - 1`, `MAX_DEPT - 1`). `MAX_NAME` veya `MAX_DEPT` değişirse bu sayılar da `query.c`, `table.c` ve `heap.c` içinde güncellenmelidir.

### Hash
- `TABLE_SIZE = 20011` (asal). 10.000 kayıtta yük faktörü α = 0.50 olur. Ödevin önerdiği 10007 ile α ≈ 1.0 olurdu. Hoca test senaryosu madde 6'yı "Yük faktörünü hesaplayınız. Yapılan aramalara etkisini yorumlayınız." olarak değiştirmiştir; etkinin yorumu için `report/olcumler.md` Tablo 2'ye bakınız.
- `hash_fn` ödevdeki gibi `static` olarak `hash_index.c` içinde tanımlanır.
- `hash_insert`: başarılıysa 0, id zaten varsa –1. Birincil anahtar benzersizliği burada denetlenir.
- `hash_update(hi, id, new_slot)`: ödevde olmayan ek fonksiyon. Silme sonrası taşınan kaydın slotunu `free`/`malloc` yapmadan günceller.
- `hash_delete`, `hash_update`: başarılıysa 0, id yoksa –1.
- Çakışma sayacı: dolu bir kovaya yapılan her ekleme 1 çakışma sayılır. Sayaç birikimlidir; silmede azaltılmaz.
- `hash_print_stats`: yük faktörü, dolu kova oranı ve toplam çakışma sayısını yazdırır.

### Heap
- `heap_insert`: başarılıysa kaydın yerleştirildiği slotu, kapasite doluysa –1 döndürür.
- `heap_delete`: son kaydı silinen kaydın yerine taşır, boşalan slotu döndürür; id yoksa –1.

### Silme kuralı (table_delete)
1. Hash'ten id'nin slotunu bul. Yoksa –1 döndür.
2. Kaydın GPA'sını oku ve AVL'den (gpa, id) çiftini sil.
3. `heap_delete` ile heap'ten sil, dönen slotu al.
4. Dönen slot < `count` ise oraya başka bir kayıt taşınmıştır. O kaydın hash'teki slotunu `hash_update` ile güncelle.
5. Silinen id'yi hash'ten çıkar.

### AVL
- Anahtar (gpa, id) çiftidir: önce GPA, eşitse id karşılaştırılır. Her düğüm benzersizdir, `avl_delete(key, id)` tam olarak tek bir düğümü hedefler.
- Aynı GPA'lı kayıtlar bir düğümün hem solunda hem sağında olabileceği için `avl_range` sınıra eşit anahtarlarda da her iki alt ağaca iner (`>= lo`, `<= hi`).
- `avl_range`: çağıran `*cnt = 0` yapar ve yeterli boyutta `out` ayırır. Fonksiyon sonuçları sona ekler.
- Yükseklik: NULL = 0, yaprak = 1. Doğrulama sınırı ⌊1.44 · log₂(n)⌋. Bu sınır küçük n'lerde (ör. n = 2) tutmayabilir; ders kitaplarındaki kesin sınır 1.44 · log₂(n+2) − 0.328'dir.
- `avl_insert` ve `avl_delete` yeni kökü döndürür. `AVLIndex.count` sayacını Table günceller.
- `avl_verify` (ödevde void) int döndürür: bulduğu hata sayısı. Her düğümde üç şart denetlenir: denge (|sol − sağ| ≤ 1), `height` alanının doğruluğu ve in-order sıralamanın (gpa, id) çiftine göre kesin artan olması. Sonuç STATS çıktısında "AVL doğrulama" satırında görünür.
- `avl_destroy` ve `hash_destroy`: bellek sızıntısı kontrolü için eklendi.

### Table
- `table_insert`: 0 başarılı, –1 id zaten var, –2 kapasite dolu.
- `table_delete`: 0 başarılı, –1 id yok.
- `table_range_gpa`, `table_select_dept`: bulunan kayıt sayısı. id'ler `out`'a yazılır, `out` en az `MAX_RECORDS` boyutunda olmalıdır.
- `table_load_csv`: eklenen kayıt sayısı, dosya açılamazsa –1. REPL'deki `LOAD` bunu kullanır. `heap_load_csv` indeksleri güncellemediği için sadece Adım 1 testlerinde kullanılır.

### Sorgu
- Komut biçimi ödevdeki gibidir: `SELECT id = 1042`, `SELECT gpa BETWEEN 3.0 3.5`, `SELECT dept = "Bilgisayar"`. Anahtar kelimeler büyük harfle yazılır.
- `query_parse` Table'a dokunmaz, sadece `Query` doldurur. Anlaşılamayan satır `Q_ERROR` olur. Satır başındaki boşluklar atlanır, boş satırlar sessizce geçilir.
- Bulunan kayıtlar `id | ad | bölüm | GPA | sınıf` biçiminde yazdırılır; 10'dan fazlaysa ilk 10'u gösterilir, toplam sayı her zaman yazılır.
- Süre sadece Table işleminin etrafında ölçülür (ekrana yazdırma dahil değildir) ve her sorgudan sonra `Süre:` satırında yazdırılır.
- Son sorgu süresi `query.c` içinde `static` bir değişkende tutulur. `STATS` bunu yazdırır ve kendisi bu değeri değiştirmez.
- `repl_set_echo(int on)`: ödevde olmayan ek fonksiyon. `--echo` seçeneği için okunan satırları ekrana yazdırmayı açar.

### Veri (CSV)
- Biçim: başlık satırı `id,name,dept,gpa,year`, alanlar virgülle ayrılır, isimlerde virgül yoktur.
- `tools/gen_csv.c` kendi rastgele sayı üretecini (LCG, tohum 42) kullanır; `rand()` platformlar arasında farklı sonuç verdiği için kullanılmamıştır. Rastgele değerler ayrı deyimlerde üretilir, çünkü C fonksiyon argümanlarının hesaplanma sırasını tanımlamaz.
- id'ler 1000–99999 aralığından benzersiz ve karışık sıradadır. GPA 0.00–4.00 arası iki basamaklıdır; aynı GPA'lı çok sayıda kayıt vardır.

### Ölçüm yöntemi (tools/bench.c)
- Her ölçüm, işlem en az 0.1 saniye sürene kadar tekrarlanır ve toplam süre işlem sayısına bölünür. Tek bir işlem `clock()` çözünürlüğünün (1 µs) altında kaldığı için bu gereklidir.
- Her turda 1000 işlem yapılır; böylece `clock()` çağrısının kendi maliyeti sonucu bozmaz.
- Sonuçlar bir `volatile` değişkende toplanır; aksi halde `-O2` derleyici ölçülen döngüyü silebilir.
- Veri, CSV üreticisiyle aynı yöntemle program içinde üretilir (yük faktörü deneyi 80.000 kayda kadar çıktığı için).

## Durum ve devir

### Tamamlananlar
- Enis: `heap.c`, `hash_index.c`, `avl_index.c`, `table.c`, `query.c` (ilk sürüm)
- Yusuf: iskelet ve sözleşme (başlıklar, Makefile, README), `avl_range` eşit anahtar düzeltmesi, ödevdeki komut biçimi, kayıt ve süre çıktısı, STATS, `sscanf` sınırları, `avl_verify` genişletmesi, CSV üreticisi, ölçüm programı (Tablo 1–5), REPL gösterimi, bellek kontrolü (macOS `leaks` ve Linux `valgrind`: 0 sızıntı)

### Kalanlar
- [ ] Big-O yorum satırları: her fonksiyonun üstüne, ödevdeki tablolara göre (Enis)
- [ ] Rapor bölüm 13, Sistem Mimarisi: yapılar arası ilişki diyagramı (Enis)
- [ ] Rapor bölüm 14, Veri Yapısı Kararları: yukarıdaki "Sözleşme kararları" taslak olarak kullanılabilir (Enis)
- [ ] Rapor bölüm 15, Performans Tablosu: `report/olcumler.md` tabloları ve "Ölçüm yöntemi" bölümü (ortak)
- [ ] Rapor bölüm 16, Big-O Analizi: teorik ile deneysel karşılaştırma (ortak)
- [ ] Rapor bölüm 17, Grup Katkı Dağılımı: yukarıdaki "Tamamlananlar" listesi ve commit geçmişi (ortak)
- [ ] `report/rapor.pdf` teslim edilecek
- [ ] Teslimden önce Linux'ta `make clean && make && make bench && make demo` ve `valgrind ./tinysql` son kontrolü (Enis)

## Git düzeni

- `main` dalı her zaman derlenebilir durumda kalır.
- Çalışmaya başlamadan önce `git pull --ff-only` ile güncel hal alınır.
- Aynı dosyaya iki kişi aynı anda dokunmaz.
- Bir sözleşme (`.h`) değişikliği gerekirse önce konuşulur, README güncellenir.
