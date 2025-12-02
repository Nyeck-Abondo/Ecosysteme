<div align = center>

# **👨‍💻ECOSYTEME_INTELLIGENT_2D**
## **🪧Description du projet**
</div>
<div align = justify>

Ce projet d'ecosysteme intelligent est un programme en C++ simulant le fonctionnement d'un ecosysteme naturel sous sa for,e la plus basique. Il intègre trois principales espèces d'être vivant: **les carnivores, les herbivores et les vegetaux**. Ce projet utilise les principes de la progra,,ation modulaire, et la programmation orientée objet en intégrant en son sein les notions d'**espace de nom; de classe; de constructeurs et de destructeurs**.
</div>

## **🗂️Architecture du projet**
```
ecosystem_simulator/
├── include/
│   ├── Core/
│   │   ├── Structs.h
│   │   ├── Entity.h
│   │   └── Ecosystem.h
│   │   │__GameEngine.h
│   │
│   └── Graphics/
│       ├── Window.h
│       └── Renderer.h
├── src/
│   ├── Core/
│   │   ├── Entity.cpp
│   │   └── Ecosystem.cpp
│   │   │__GameEngine.cpp
│   │
│   ├── Graphics/
│   │   ├── Window.cpp
│   │   └── Renderer.cpp
│   └── main.cpp
├── assets/
│   └── (futures textures)
└── README.md
```
## **ESPACES DE NOM ET CLASSES DU PROJET**
Ce projet utilise de nombreux aspects de la programmation orienté objet à l'instar des **namespace** et des **classes**.
### **NAMESPACE**
```cpp
namespace Ecosystem {
    namespace Core {
        /*code*/
    }
}
/**
 * contient l'implémentation des différentes classes
 * :Entity et ecoystem
 * contient l'implémentation des structures de base 
 * :Vector2D; Food; color
*/
```
<div align = justify>

Présent à la fois dans les fichiers **include/Core/```Ecosystem.h```** et **include/Core/```Entity.h```**, cet espace de nom permet d'éviter les conflits de nom lors des appels des méthodes relatives aux différentes classes : **```Entity```** ou encore **```Ecosystem```** dans les divers modules du programme.
</div>

### **🗄️LES CLASSES**
#### **include/Vore/Ecosystem.h**
```cpp
class Ecosystem {
    private:🔏
    /**
     * liste des variables / attribus privées
    */
   public:
   /**
    * liste des variables / attribus publiques
    * prototype du constructeur
    * prototype du destructeur
    * Methodes publiques
    * implementation des Getters
    * Methodes de gestion
    * Rendu
   */
  private:🔏
  /**
   * Methodes privees
  */
}
```
<div align = justify>

**La classe ```Ecosystem```** est un modèle permettant la gestion dynamique de l'écosystème basique en intégrant des attribus important à l'instar de: 

- **```mEntities```** de type **``` std::vector<std::unique_ptr<Entity>>```** qui est un tableau dynamique de type **```Entity```** effectuqnt le ressencement de l'ensemble des entités de l'écosystème. Il sert notemment dans le programme à gérer les interactions entre les entités **herbivores** et **carnivore** de l'environnement.
- **```mFoodSources```** de type **```std::vector<Food>```** : qui est un tableau dynamique qui enregistre les différentes nourritures présentes sur le terrain de l'écosystème. il est notamment utiliser dans la fonction de logique de recherche de nourriture par les **herbivores** afin d'assurer leur survie par alimentation.

pour ne citer que celles là.
</div>

#### **📝include/Core/Entity.h**
```cpp
class Entity {
    private:🔏🔏
    /**
     * déclaration des attribus
     * création du générateur aléatoire
    */
   public:
   /**
    * déclaration des données publiques
    * prototype du constructeur
    * prototype du destructeur
    * prototypees des méthodes publiques
    * impléentation des Getters
    * prototypes des ?éthodes de comportement des entités
    * Methode de rendu
    * Méthodes privées
   */
}
```
<p align = justify>

**La classe ```Entity```** quant à elle est un modèle décrivant la structure des être constituants l'ecosysteme. Ses attributs les plus utilisés dans l'implémentation des fonctions demandées pour ce projet sont principalement:

- ```mEnergy``` de type ```float```, qui permet de vérifier l'état de la vitalité de l'entité. s'il est null, l'entité meurt et est dégagée de l'environnement.

- ```mVelovity``` de type **```Vector2D```**: il représente le vecteur accélération de l'entité courante (à l'exception des plante qui elles restent sur place tout le long de la simulation)

- **```mType```** de type **```EntityType```**: qui difini le type de l'entité entre trois grqnd choix d'èspeces (**HERBIVOR, CARNIVOR; PLANT**) cela permet une meilleure distinction lors des manipulation des logiques de comportement.
</p>
<div align = center>

## **🗂️FICHIERS ET DESCRIPTION DES IMPLEMENTATIONS**
<div align = justify>

## **👾Fonction decomportement**
### **📝src/Core/Entity.cpp**
```cpp
/**
 * SeekFood
 * @brief: C'est la logique de recherche de lq nourriture pour les entiés de type
 * herbivore. elle cqlcule un vecteur de déplacement par rapport à la 
 * nourriture la plus proche avant de le retourner sous sa forme normalisée
 * @param: const std::vector<Food>& foodRsources : ceci est le tableau contenant
 * l'ensembles des nourritures presents dans l'écosystème
 * @return: posTemp - c'est le vecteur normalié formé entre l'herbivore et la 
 * nourriture la plus proche
 */
Vector2D Entity::SeekFood(const std::vector<Food>& foodRsources) {
    int count;
    //recuperation de la position de lherbivore
    Vector2D posTemp = position; 
    Vector2D foodPos;
    float distMin = 2000.0f;
    float distance;
    for (auto& food : foodRsources) {
        distance = posTemp.Distance(food.position);
        if (distMin > distance && distance < 300.0f) {
            distMin = distance;
            //position de la nourriture la plus proche
            foodPos = food.position;
        }
    }
    posTemp.x = foodPos.x - posTemp.x;
    posTemp.y = foodPos.y - posTemp.y;
    float length = std::sqrt(posTemp.x * posTemp.x + posTemp.y * posTemp.y);
    //normalisation du vecteur de deplacement
    posTemp.x /= length;
    posTemp.y /= length;
    return posTemp;
}
```
Cette méthode **```Entity::SeekFood()```** est la version dédiée aux herbivores et leur permet de se diriger directement vers leur nourriture

```cpp
/**
 * SeekFood
 * @brief: C'est la logique de recherche de lq nourriture pour les entiés de type
 * herbivore. elle calcule un vecteur de déplacement par rapport aux
 * herbivores les plus proches avant de le retourner sous sa forme normalisée
 * @param: const std::vector<std::unique_ptr<Entity>>& EntityFood : ce paramètre 
 * représente le tqblequ dynq,iaue contenant l'ensemble des entiées de l'écosystème
 * @return: predatorPos : c'est le vecterur normaliser le la position du prédateur 
 * par rapport à l'herbivore
 */
Vector2D Entity::SeekFood(const std::vector<std::unique_ptr<Entity>>& EntityFood) {
    //recuperation de la position du carnivore courant
    Vector2D predatorPos = position;
    Vector2D foodPos;
    float Energie = mEnergy;
    float distMin = 10000.0f;
    float dist;
    for (auto& entity : EntityFood) {
        if (entity->GetType() == EntityType::HERBIVORE) {
            dist = predatorPos.Distance(entity->position);
            if (distMin >= dist) {
                distMin = dist;
                //position de l'herbivore le plus proche
                foodPos = entity->position;
            }
        }
    }
    herbipos.x = herbipos.x - predatorPos.x;
    herbipos.y = herbipos.y - predatorPos.y;
    float length = std::sqrt(herbipos.x * herbipos.x + herbipos.y * herbipos.y);
    //normalisation du vecteur de deplacement
    herbipos.x /= length;
    herbipos.y /= length;
    return predatorPos;
}
```
Cette méthode **```Entity::SeekFood()```** est la version dédiée aux carnivores et leur permet de se diriger directement vers leur nourriture en l'occurence les herbivores. on calcule tout d'abord la distance entre chaque element herbivore du tableau d'entité et le carnivore courant, puis on retourne le vecteur nor,aliser que devra suivre le carnivore pour atteindre l'herbivore.

```cpp
/**
  * StayInBounds - fait en sorte que les entités ne sortent pas de la fenètre
  * @param: wordlHeight : hauteur de la fenetre
  * @param: worldWidth : largeur de la fenetre
  * @return: position : les coordonnées pqr defqut en cqs de depassement
  * des bordures de la fenètre SDl
  */
Vector2D Entity::StayInBounds(float worldWidth, float worldHeight) {
    if (position.x > worldWidth - 6.0f) position.x = worldWidth - 6.0f;
    if (position.y > worldHeight - 6.0f) position.y = worldHeight - 6.0f;
    if (position.x < 6.0f) position.x = 6.0f;
    if (position.y < 6.0f) position.y = 6.0f;
    return position;
}
```
**```Entity::StayInBounds()```** permet aux entites de rester dqns le cqdre de la fenetre quoi qu'il arrive.

```cpp
/**
 * Applyforce - applique la force de déplacement
 * @param: force : repésente la force à appliquer sur l'entité
 */
void Entity::ApplyForce(Vector2D force) {
    mVelocity = mVelocity + force;
    float length = std::sqrt(mVelocity.x * mVelocity.x + mVelocity.y * mVelocity.y);
    mVelocity.x /= length;
    mVelocity.y /= length;
}
```
**```Entity::ApplyForce()```** applique la force désirée en modifiant directement la velocité

```cpp
/**
 * Die - tue l'entié en vidant ses réserves d'énergie
 */
void Entity::Die() {
    mEnergy = 0.0f;
}
```
la methode **```Entity::Die()```** n'existe initialement pas dans le projet fourni par le professeur. Elle a été créée et implémentée dans le but de pouvoir tuer plus facilement les entités mangées (les HERBIVORES). elle retire l'entièreté de leur énergie; les faisant ainsi passées pour mortes de façon prématurée.

```cpp
/**
 * AvoidPredators
 * @brief: cette fonction permet au entités du type HERBIVOR de fuir les CARNIVORS
 * quand ceux ci se trouvent dans un certain périmètre autour d'eux
 * @param: const std::vector<std::unique_ptr<Entity>>& EntityFood : ce paramètre 
 * représente le tableau dynamiaue contenant l'ensemble des entiées de l'écosystème
 * @return: herbipos : c'est l;inverse du vecteur carnivore -herbivore
 */
Vector2D Entity::AvoidPredators(const std::vector<std::unique_ptr<Entity>>& predators) const {
    //position de l'herbivore courant
    Vector2D herbipos = position ;
    Vector2D predatorPos;
    float distMin = 3000.0f;
    float dist;
    for (auto& entity : predators) {
        if (entity->GetType() == EntityType::CARNIVORE) {
            dist = herbipos.Distance(entity->position);
            if (distMin > dist && dist < 30.0f) {
                distMin = dist;
                //position du carnivore le plus proche
                predatorPos = entity->position;
            }
        }
    }
    herbipos.x = herbipos.x - predatorPos.x;
    herbipos.y = herbipos.y - predatorPos.y;
    float length = std::sqrt(herbipos.x * herbipos.x + herbipos.y * herbipos.y);
    //normalisation du vecteur de deplacement
    herbipos.x /= length;
    herbipos.y /= length; 
    return herbipos;
}
```
</div>
<div align = justify>

# **👨‍💻code de compilation**

```
 g++ -std=c++17 -Iinclude -o ecosystem src/*.cpp src/Core/*.cpp src/Graphics/*.cpp -o ecosysteme.exe -lSDl3
 ```
 ## **👨‍🏫resultat attendu**
 

 Une fenetre dans laquelle se déplqce des carnivore en rouge 🟥et herbivore en bleu 🟦. Les carnivore chassant les herbivores lorsque leur niveau d'energie est bas et des herbivores cherchant de la nourriture quand le leur l'est aussi. les herbivorres se reproduisent selon un taux de chance plus élevé aue celui des carnivores et fuient ceux ci tant qu'ils n'ont pas faim et quand ils se trouvent dans un certain perimetre.
 </div>