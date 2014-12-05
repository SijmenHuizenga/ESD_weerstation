Weerstation Prototype
=====================

Dit is een projectje voor Embeded Software Design. Het doel is om een
prototype voor een weerstation te maken. Het meet gedeelte wordt
verzorgd door een arduino. De arduino stuurt de data live via een 
seriele poort naar een computer. Door middel van een processing applicatie
kan deze data gecotroleerd worden (checksum) en daarna worden weergegeven.

Ecipse
---------
Beide onderdelen(arduion en processing) zijn developed in Eclipse. Het 
programma kan dus ook niet zomaar via de processing of arduino ide worden 
uitgevoerd. Als je dit toch zou willen doen, moet er wat knip en plak
werk worden gedaan.

Dependencies
----------
De code van processing is geschreven in Java met de processing library 
geïmporteerd. Het gaat om de core.jar, serial/serial.jar en serial/jssc.jar.