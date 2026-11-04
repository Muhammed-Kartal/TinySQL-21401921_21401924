# TinySQL Ölçüm Sonuçları

## Tablo 1: Nokta sorgusu (Test senaryosu, madde 1-5)

1000 rastgele mevcut id aranmıştır; süreler işlem başına ortalamadır.

| n | Heap araması O(n) (ns) | Hash araması O(1) (ns) | Hızlanma |
|---:|---:|---:|---:|
| 100 | 23.6 | 2.0 | 12x |
| 1000 | 139.2 | 3.0 | 47x |
| 10000 | 1613.7 | 3.8 | 427x |

## Tablo 2: Yük faktörünün aramaya etkisi (Test senaryosu, madde 6)

Kova sayısı sabit (TABLE_SIZE = 20011), kayıt sayısı artırılarak α değiştirilmiştir. Başarılı aramada beklenen karşılaştırma sayısı teoride 1 + α/2'dir. n = 10000 satırı madde 6'nın istediği değerdir; diğer satırlar etkiyi yorumlamak için yapılan ek ölçümdür.

| n | α | Dolu kova oranı | En uzun zincir | Ort. karşılaştırma (ölçülen) | Ort. karşılaştırma (teori) | Hash araması (ns) |
|---:|---:|---:|---:|---:|---:|---:|
| 5000 | 0.25 | 0.22 | 4 | 1.122 | 1.125 | 3.3 |
| 10000 | 0.50 | 0.39 | 5 | 1.258 | 1.250 | 3.6 |
| 14000 | 0.70 | 0.50 | 6 | 1.350 | 1.350 | 3.6 |
| 20000 | 1.00 | 0.64 | 7 | 1.492 | 1.500 | 3.7 |
| 40000 | 2.00 | 0.87 | 9 | 1.995 | 1.999 | 5.2 |
| 80000 | 4.00 | 0.98 | 15 | 2.994 | 2.999 | 6.9 |

## Tablo 3: AVL yüksekliği ve doğrulama (Test senaryosu, madde 7 ve 9)

| n | AVL yüksekliği | Teorik üst sınır ⌊1.44·log₂n⌋ | Sınır içinde | avl_verify hata sayısı |
|---:|---:|---:|:---:|---:|
| 100 | 8 | 9 | evet | 0 |
| 1000 | 12 | 14 | evet | 0 |
| 10000 | 16 | 19 | evet | 0 |

## Tablo 4: Aralık sorgusu GPA ∈ [3.0, 3.5] (Test senaryosu, madde 8)

Süreler sorgu başına ortalamadır. k: bulunan kayıt sayısı.

| n | k (AVL) | k (doğrusal) | AVL O(log n + k) (µs) | Doğrusal tarama O(n) (µs) | Hızlanma |
|---:|---:|---:|---:|---:|---:|
| 100 | 16 | 16 | 0.047 | 0.067 | 1.4x |
| 1000 | 137 | 137 | 0.303 | 0.581 | 1.9x |
| 10000 | 1215 | 1215 | 3.340 | 20.017 | 6.0x |

## Tablo 5: Aynı sonucu veren SELECT id ve SELECT dept (Test senaryosu, madde 12)

Tabloya bölümü "Felsefe" olan tek bir kayıt (id = 100500) eklenmiştir. Böylece SELECT id = 100500 ve SELECT dept = "Felsefe" aynı tek kaydı döndürür.

| n | SELECT id sonucu | SELECT dept sonucu | SELECT id, hash (ns) | SELECT dept, tarama (ns) | Fark |
|---:|:---:|:---:|---:|---:|---:|
| 101 | id 100500 | 1 kayıt (id 100500) | 2.5 | 212.3 | 85x |
| 1001 | id 100500 | 1 kayıt (id 100500) | 2.7 | 1941.8 | 712x |
| 10001 | id 100500 | 1 kayıt (id 100500) | 2.4 | 18191.8 | 7498x |

