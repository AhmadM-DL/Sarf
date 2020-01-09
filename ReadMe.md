
# Sarf #

Sarf is an Arabic Morphological Analysis Tool based on c++.

## Examples ##

Feeding "يكتبها" to the stemmer class will return the following results.
 
Results:

ALTERNATIVE:	 يُكْتِبها

|Morphological Type | Text | Diacritic Text | Glossary            | Part of Speech      |
| ----------------- |:----:| :-------------:|:-------------------:|:-------------------:|
|PREFIX:   	    | ي  |   يُ              |"he/it"              | yu/IV3MS+ |
|STEM:	            |كتب |  كْتِب             |"dictate/make write" | kotib/VERB_IMPERFECT|
|SUFFIX:	    |ها |    ها	             |"it/them/her"	  | +hA/IVSUFF_DO:3FS|


ALTERNATIVE:	 يَكْتُبها

|Morphological Type | Text | Diacritic Text | Glossary            | Part of Speech      |
| ----------------- |:----:| :-------------:|:-------------------:|:-------------------:|
|PREFIX:            |	ي|  يَ	            |"he/it"	          |ya/IV3MS+|
|STEM:              |	كتب|كْتُب            |"write"	          |kotub/VERB_IMPERFECT|
|SUFFIX:            |	ها|  ها	            |"it/them/her"	  |+hA/IVSUFF_DO:3FS|

## Installation ##

### Linux ###

- Clone this Repo.
- Run install.sh (run as Adminstrator)

```
# git clone "https://github.com/AhmadM-DL/Sarf.git"
# cd Sarf
# sudo ./install.sh
```

