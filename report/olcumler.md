# TinySQL Ölçüm Sonuçları

## Tablo 1: Nokta sorgusu (Kabul 1-5)

1000 rastgele mevcut id aranmıştır; süreler işlem başına ortalamadır.

| n | Heap araması O(n) (ns) | Hash araması O(1) (ns) | Hızlanma |
|---:|---:|---:|---:|
| 100 | 30.0 | 2.2 | 14x |
| 1000 | 173.6 | 2.5 | 69x |
| 10000 | 3569.4 | 2.7 | 1337x |

## Tablo 2: Yük faktörünün aramaya etkisi (Kabul 6)

Kova sayısı sabit (TABLE_SIZE = 20011), kayıt sayısı artırılarak α değiştirilmiştir. Başarılı aramada beklenen karşılaştırma sayısı teoride 1 + α/2'dir.

| n | α | Dolu kova oranı | En uzun zincir | Ort. karşılaştırma (ölçülen) | Ort. karşılaştırma (teori) | Hash araması (ns) |
|---:|---:|---:|---:|---:|---:|---:|
| 5000 | 0.25 | 0.22 | 4 | 1.122 | 1.125 | 3.2 |
| 10000 | 0.50 | 0.39 | 5 | 1.258 | 1.250 | 2.8 |
| 14000 | 0.70 | 0.50 | 6 | 1.350 | 1.350 | 2.7 |
| 20000 | 1.00 | 0.64 | 7 | 1.492 | 1.500 | 2.8 |
| 40000 | 2.00 | 0.87 | 9 | 1.995 | 1.999 | 3.6 |
| 80000 | 4.00 | 0.98 | 15 | 2.994 | 2.999 | 5.1 |

