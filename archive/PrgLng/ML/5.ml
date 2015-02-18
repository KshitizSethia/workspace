fun merge comparer [] L2 = L2
 |  merge comparer L1 [] = L1
 |  merge comparer (e1::L1s) (e2::L2s) = if (comparer (e1,e2))
										then e1::merge comparer L1s (e2::L2s)
										else e2::merge comparer (e1::L1s) L2s;