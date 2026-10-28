# TinySQL Ölçüm Sonuçları

## Tablo 1: Nokta sorgusu (Kabul 1-5)

1000 rastgele mevcut id aranmıştır; süreler işlem başına ortalamadır.

| n | Heap araması O(n) (ns) | Hash araması O(1) (ns) | Hızlanma |
|---:|---:|---:|---:|
| 100 | 32.0 | 2.3 | 14x |
| 1000 | 171.3 | 2.5 | 68x |
| 10000 | 4051.8 | 2.7 | 1526x |

