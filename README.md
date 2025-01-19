IoT Vehicle Protection System

Description

Ce projet est un système de protection IoT innovant et complet, conçu pour améliorer la sécurité des véhicules. En combinant des capteurs, l'intelligence artificielle et une application mobile, il offre une surveillance en temps réel, des notifications instantanées et des fonctionnalités avancées de reconnaissance faciale.

Fonctionnalités principales

1. Détection et surveillance

Détection de feu et de gaz : Surveillance continue avec capteurs de flamme et de gaz (MQ2), envoi d'alertes en cas de danger.

Surveillance de proximité : Capteur de proximité pour détecter les mouvements à proximité du véhicule.

Vidéo en temps réel : La caméra s'active automatiquement lors d'une intrusion ou d'une anomalie.

2. Reconnaissance faciale avec IA

Modèle de reconnaissance faciale : Développé en Python, ce modèle prend en input une photo du propriétaire et détecte si le visage capturé par la caméra correspond à celui enregistré.

Classification des visages :

Si le visage correspond à celui du propriétaire : « Accès autorisé ».

Sinon : « Unknown », avec envoi d'une notification d'alerte à l'utilisateur.

3. Application mobile

Authentification : L'utilisateur se connecte à l'application via ses identifiants.

Récupération des données Firebase : L'application interagit avec une base de données Firebase pour récupérer les événements et notifications.

Notifications en temps réel : Alerte instantanée sur les anomalies ou intrusions.

Matériel utilisé

ESP32 : Microcontrôleur pour la gestion des capteurs et de la connectivité.

Capteur de flamme : Surveillance des risques d'incendie.

Capteur de gaz (MQ2) : Détection de gaz dangereux.

Capteur de proximité : Pour détecter les objets ou personnes proches du véhicule.

Caméra : Capture et analyse des visages.

2 lampes : Alertes visuelles.

2 résistances : Gestion électrique du circuit.

Module d'affichage : Affichage des états du système et alertes.

Architecture du projet

Système IoT : Capteurs connectés à l'ESP32 pour une surveillance en temps réel.

Reconnaissance faciale : Modèle Python analysant les flux vidéo.

Base de données Firebase : Stockage et synchronisation des données d'événements.

Application mobile React Native : Interface utilisateur intuitive pour la surveillance et la gestion du système.

Objectif

Ce projet vise à offrir une solution de sécurité complète pour les véhicules, combinant technologies IoT, intelligence artificielle et une interface utilisateur moderne. Il permet de protéger efficacement les biens et d'assurer une expérience utilisateur fluide et sécurisée.

Installation et déploiement

Matériel :

Connectez les capteurs, la caméra, et les composants au microcontrôleur ESP32 selon le schéma du circuit.

Application mobile :

Clonez le projet React Native et configurez Firebase.

Installez les dépendances et exécutez l'application sur votre appareil mobile.

Modèle IA :

Installez Python et les bibliothèques nécessaires (à définir dans un fichier requirements.txt).

Entraînez le modèle de reconnaissance faciale ou utilisez le modèle pré-entraîné.
