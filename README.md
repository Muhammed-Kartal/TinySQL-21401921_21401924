# TinySQL – Mini Veritabanı Motoru

INF224 Proje Ödevi. Hash tablosu, AVL ağacı ve dizi tabanlı heap dosyası kullanan tek tablolu bir veritabanı motoru.

## Derleme ve çalıştırma

- `make` → derler, `tinysql` programını oluşturur
- `./tinysql` → programı başlatır
- `make debug` → bellek hatası kontrolüyle (`-fsanitize=address`) derler
- `make clean` → derleme çıktılarını siler

Linux'ta bellek sızıntısı kontrolü için `valgrind ./tinysql` da kullanılabilir. macOS (Apple Silicon) üzerinde valgrind çalışmaz, `make debug` kullanılmalı.

## Dosya yapısı

- `src/tinysql.h` → `Record` yapısı ve ortak sabitler
- `src/heap.*` → Adım 1: dizi tabanlı depolama
- `src/hash_index.*` → Adım 2: id üzerinde hash indeksi
- `src/avl_index.*` → Adım 3: GPA üzerinde AVL indeksi
- `src/table.*` → üç yapıyı tutarlı tutan entegrasyon katmanı
- `src/query.*` → Adım 4: ayrıştırıcı, sorgu yürütücü, REPL
- `src/main.c` → giriş noktası

## Sözleşme kararları

### Genel
- Table dışındaki hiçbir kod heap, hash veya AVL'ye doğrudan yazmaz. Tüm ekleme ve silme işlemleri Table fonksiyonlarından geçer.
- Table yaklaşık 10 MB olduğu için `table_create` ile `malloc` üzerinden ayrılır, stack'te tutulmaz.
- Stub'lar başarısızlık değeri döndürür (–1 veya NULL). Yazılmamış bir fonksiyon asla başarılı görünmemelidir.
- Bir dosya neye ihtiyaç duyuyorsa onu kendisi include eder. Standart başlıklar `.h` dosyalarına değil, ihtiyaç duyan `.c` dosyalarına eklenir.

### Hash
- `TABLE_SIZE = 20011` (asal). Ödevin önerdiği 10007 ile 10.000 kayıtta yük faktörü ≈ 1.0 olur ve α ≤ 0.7 şartı sağlanmaz. Kapasite ≥ n / 0.7 olmalıdır. 20011 ile 14.000 kayda kadar α < 0.7 kalır. Rehash yerine bu yol seçildi çünkü ödevdeki sabit boyutlu `buckets` dizisi korunuyor.
- `hash_fn` ödevdeki gibi `static` olarak `hash_index.c` içinde tanımlanır.
- `hash_insert`: başarılıysa 0, id zaten varsa –1. Birincil anahtar benzersizliği burada denetlenir.
- `hash_update(hi, id, new_slot)`: ödevde olmayan ek fonksiyon. Silme sonrası taşınan kaydın slotunu `free`/`malloc` yapmadan günceller.
- `hash_delete`, `hash_update`: başarılıysa 0, id yoksa –1.
- Çakışma sayacı: dolu bir kovaya yapılan her ekleme 1 çakışma sayılır.

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
- Yükseklik: NULL = 0, yaprak = 1. Doğrulama sınırı ⌊1.44 · log₂(n)⌋.
- `avl_insert` ve `avl_delete` yeni kökü döndürür. `AVLIndex.count` sayacını Table günceller.
- `avl_verify` (ödevde void) int döndürür: bulduğu hata sayısı. Stub –1 döndürür ("henüz doğrulanamadı").
- `avl_range`: çağıran `*cnt = 0` yapar ve yeterli boyutta `out` ayırır. Fonksiyon sonuçları sona ekler.
- `avl_destroy` ve `hash_destroy`: bellek sızıntısı kontrolü için eklendi.

### Table
- `table_insert`: 0 başarılı, –1 id zaten var, –2 kapasite dolu.
- `table_delete`: 0 başarılı, –1 id yok.
- `table_range_gpa`, `table_select_dept`: bulunan kayıt sayısı. id'ler `out`'a yazılır, `out` en az `MAX_RECORDS` boyutunda olmalıdır.
- `table_load_csv`: eklenen kayıt sayısı, dosya açılamazsa –1. REPL'deki `LOAD` bunu kullanır. `heap_load_csv` indeksleri güncellemediği için sadece Adım 1 testlerinde kullanılır.

### Sorgu
- `query_parse` Table'a dokunmaz, sadece `Query` doldurur. Anlaşılamayan satır `Q_ERROR` olur.
- Son sorgu süresi `query.c` içinde `static` bir değişkende tutulur. `STATS` bunu yazdırır.

## Açık kararlar (birlikte karar verilecek)

- [ ] Rol dağılımı: A (heap, hash, table, CSV üretici, ölçümler) / B (AVL, query parse + REPL + exec)
- [ ] CSV formatı. Öneri: başlık satırı `id,name,dept,gpa,year`, isimlerde virgül yok
- [ ] Komut anahtar kelimeleri büyük/küçük harfe duyarlı mı? (`select` = `SELECT`?)
- [ ] Big-O yorumları her fonksiyonun üstüne, ödevdeki tabloya göre eklenecek

## Git düzeni

- `main` dalı her zaman derlenebilir durumda kalır.
- Herkes kendi dalında çalışır. Aynı dosyaya iki kişi aynı anda dokunmaz.
- Bir sözleşme (`.h`) değişikliği gerekirse önce konuşulur, README güncellenir.