# NGram-CPP
You can also see either [Java](https://github.com/olcaytaner/NGram) 
or [Python](https://github.com/olcaytaner/NGram-Py) repository.

Detailed Description
============
+ [Training NGram](#training-ngram)
+ [Using NGram](#using-ngram)
+ [Saving NGram](#saving-ngram)
+ [Loading NGram](#loading-ngram)

## Training NGram
     
Boş bir NGram modeli oluşturmak için

	NGram(int N)

Örneğin,

	a = NGram(2);

boş bir bigram modeli oluşturulmaktadır.

NGram'a bir cümle eklemek için

	void addNGramSentence(Symbol[] symbols)

Örneğin,

	String[] text1 = {"ali", "topu", "at", "mehmet", "ayşe", "gitti"};
	String[] text2 = {"ali", "top", "at", "ayşe", "gitti"};
	nGram = new NGram<String>(2);
	nGram.addNGramSentence(text1);
	nGram.addNGramSentence(text2);

satırları ile boş bir bigram oluşturulup, text1 ve text2 cümleleri bigram modeline 
eklenir.

NoSmoothing sınıfı smoothing için kullanılan en basit tekniktir. Eğitim gerektirmez, sadece
sayaçlar kullanılarak olasılıklar hesaplanır. Örneğin verilen bir NGram'ın NoSmoothing ile 
olasılıklarının hesaplanması için

	a.calculateNGramProbabilities(new NoSmoothing());

LaplaceSmoothing sınıfı smoothing için kullanılan basit bir yumuşatma tekniğidir. Eğitim 
gerektirmez, her sayaca 1 eklenerek olasılıklar hesaplanır. Örneğin verilen bir NGram'ın 
LaplaceSmoothing ile olasılıklarının hesaplanması için

	a.calculateNGramProbabilities(new LaplaceSmoothing());

GoodTuringSmoothing sınıfı smoothing için kullanılan eğitim gerektirmeyen karmaşık bir 
yumuşatma tekniğidir. Verilen bir NGram'ın GoodTuringSmoothing ile olasılıklarının 
hesaplanması için

	a.calculateNGramProbabilities(new GoodTuringSmoothing());

AdditiveSmoothing sınıfı smoothing için kullanılan eğitim gerektiren bir yumuşatma 
tekniğidir.

	a.calculateNGramProbabilities(new AdditiveSmoothing());

## Using NGram

Bir NGram'ın olasılığını bulmak için

	double getProbability(Symbol ... symbols)

Örneğin, bigram olasılığını bulmak için

	a.getProbability("ali", "topu")

trigram olasılığını bulmak için

	a.getProbability("ali", "topu", "at")

## Saving NGram
    
NGram modelini kaydetmek için

	void saveAsText(String fileName)

Örneğin, a modelini "model.txt" dosyasına kaydetmek için

	a.saveAsText("model.txt");              

## Loading NGram            

Var olan bir NGram modelini yüklemek için

	NGram(String fileName)

Örneğin,

	a = NGram("model.txt");

model.txt dosyasında bulunan bir NGram modelini yükler.
