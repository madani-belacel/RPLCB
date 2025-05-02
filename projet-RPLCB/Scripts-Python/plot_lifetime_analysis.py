
import pandas as pd
import matplotlib.pyplot as plt

# Charger les données
df = pd.read_csv("lifetime_simulated.csv")

# Calcul de la durée de vie moyenne par taille de réseau
avg_lifetime = df.groupby("Node Count")["Lifetime (s)"].mean().reset_index()

# Calcul de la durée de vie du réseau (premier nœud mort)
network_lifetime = df[df["Status"] == "Dead"].groupby("Node Count")["Lifetime (s)"].min().reset_index()

# Tracer les courbes

# 1. Durée de vie moyenne des nœuds (en anglais)
plt.figure(figsize=(8, 5))
plt.plot(avg_lifetime["Node Count"], avg_lifetime["Lifetime (s)"], marker='o', color='blue')
plt.title("Average Node Lifetime vs Number of Nodes")
plt.xlabel("Number of Nodes")
plt.ylabel("Average Node Lifetime (s)")
plt.grid(True)
plt.tight_layout()
plt.savefig("node_lifetime_en.png")

# 2. Durée de vie du réseau (en anglais)
plt.figure(figsize=(8, 5))
plt.plot(network_lifetime["Node Count"], network_lifetime["Lifetime (s)"], marker='o', color='red')
plt.title("Network Lifetime vs Number of Nodes")
plt.xlabel("Number of Nodes")
plt.ylabel("Network Lifetime (s)")
plt.grid(True)
plt.tight_layout()
plt.savefig("network_lifetime_en.png")

# 3. Durée de vie moyenne des nœuds (en français)
plt.figure(figsize=(8, 5))
plt.plot(avg_lifetime["Node Count"], avg_lifetime["Lifetime (s)"], marker='o', color='blue')
plt.title("Durée de vie moyenne des nœuds selon le nombre de nœuds")
plt.xlabel("Nombre de nœuds")
plt.ylabel("Durée de vie moyenne (s)")
plt.grid(True)
plt.tight_layout()
plt.savefig("node_lifetime_fr.png")

# 4. Durée de vie du réseau (en français)
plt.figure(figsize=(8, 5))
plt.plot(network_lifetime["Node Count"], network_lifetime["Lifetime (s)"], marker='o', color='red')
plt.title("Durée de vie du réseau selon le nombre de nœuds")
plt.xlabel("Nombre de nœuds")
plt.ylabel("Durée de vie du réseau (s)")
plt.grid(True)
plt.tight_layout()
plt.savefig("network_lifetime_fr.png")
