import React, { useState, useEffect } from "react";
import { View, Text, StyleSheet, FlatList, ActivityIndicator } from "react-native";

export default function Home() {
  const [data, setData] = useState([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState(null);

  useEffect(() => {
    fetch("https://projectiotapplication-default-rtdb.firebaseio.com/.json")
      .then((response) => response.json())
      .then((json) => {
        console.log("Données brutes :", json); // Affichez les données brutes

        if (json && json.fire && json.fire.devices) {
          const devices = Object.entries(json.fire.devices).map(([key, value]) => ({
            id: key, // Utilisez la clé comme identifiant
            ...value, // Ajoutez les propriétés de l'appareil
          }));
          console.log("Données formatées :", devices); // Affichez les données formatées
          setData(devices);
        } else {
          setData([]); // Si aucune donnée n'existe
        }

        setLoading(false);
      })
      .catch((error) => {
        console.error("Erreur lors de la récupération des données :", error);
        setError("Une erreur s'est produite lors de la récupération des données.");
        setLoading(false);
      });
  }, []);

  if (loading) {
    return (
      <View style={styles.loadingContainer}>
        <ActivityIndicator size="large" color="blue" />
        <Text>Chargement des données...</Text>
      </View>
    );
  }

  if (error) {
    return (
      <View style={styles.loadingContainer}>
        <Text style={{ color: "red" }}>{error}</Text>
      </View>
    );
  }

  return (
    <View style={styles.container}>
      <Text style={styles.title}>Données récupérées :</Text>
      {data.length === 0 ? (
        <Text>Aucune donnée disponible.</Text>
      ) : (
        <FlatList
          data={data}
          keyExtractor={(item) => item.id}
          renderItem={({ item }) => (
            <View style={styles.item}>
              <Text style={styles.itemTitle}>{item.name || "Sans nom"}</Text>
              <Text>Status : {item.status || "Inconnu"}</Text>
              <Text>Valeur : {item.value || "Non spécifiée"}</Text>
            </View>
          )}
        />
      )}
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    padding: 16,
  },
  loadingContainer: {
    flex: 1,
    justifyContent: "center",
    alignItems: "center",
  },
  title: {
    fontSize: 20,
    fontWeight: "bold",
    marginBottom: 16,
  },
  item: {
    padding: 10,
    borderBottomWidth: 1,
    borderBottomColor: "#ccc",
  },
  itemTitle: {
    fontSize: 16,
    fontWeight: "bold",
  },
});
