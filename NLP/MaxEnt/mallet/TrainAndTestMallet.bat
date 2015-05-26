cd C:\Cloud\github\workspace\LanguageStore\java_jars\mallet-2.0.7\lib
::training
java -cp "..\class;mallet-deps.jar" cc.mallet.fst.SimpleTagger --model-file C:\Cloud\github\workspace\NLP\MaxEnt\mallet\model.dat --train true --threads 8 --training-proportion 0.9 C:\Cloud\github\workspace\NLP\MaxEnt\mallet\training_features_mallet.dat 1>C:\Cloud\github\workspace\NLP\MaxEnt\mallet\training_log.txt 2>&1
::testing
java -cp "..\class;mallet-deps.jar" cc.mallet.fst.SimpleTagger --model-file C:\Cloud\github\workspace\NLP\MaxEnt\mallet\model.dat C:\Cloud\github\workspace\NLP\MaxEnt\mallet\test_features_mallet.dat 1>C:\Cloud\github\workspace\NLP\MaxEnt\mallet\mallet_test_output.txt
::evaluating model
cd C:\Cloud\github\workspace\NLP\MaxEnt
java bin\MalletEvaluate.class
pause