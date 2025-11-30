# **ECOSYTEME_INTELLIGENT_2D**
## **Description du projet**
Ce projet d'ecosysteme intelligent est un programme en C++ simulant le fonctionnement d'un ecosysteme naturel sous sa for,e la plus basique. Il intègre trois principales espèces d'être vivant: **les carnivores, les herbivores et les vegetaux**. Ce projet utilise les principes de la progra,,ation modulaire, et la programmation orientée objet en intégrant en son sein les notions d'**espace de nom; de classe; de constructeurs et de destructeurs**.

## **Architecture du projet**
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

## **Fichiers et description**

### **Ecosystem.h**
```cpp
#pragma once

#include "Entity.h" 
#include "Structs.h" 
#include <vector> 
#include <memory> 
#include <random> 

namespace Ecosystem { 
namespace Core { 
class Ecosystem {
private: 
    //ÉTAT INTERNE 
    std::vector<std::unique_ptr<Entity>> mEntities;
    std::vector<Food> mFoodSources; 
    float mWorldWidth; 
    float mWorldHeight; 
    int mMaxEntities;
    int mDayCycle;
    //Générateur aléatoire 
    mutable std::mt19937 mRandomGenerator;
    //STATISTIQUES
    struct Statistics {
        int totalHerbivores;
        int totalCarnivores;
        int totalPlants;
        int totalFood;
        int deathsToday;
        int birthsToday;
    } mStats; 
public:
    //CONSTRUCTEUR/DESTRUCTEUR 
    Ecosystem(float width, float height, int maxEntities = 500); 
    ~Ecosystem(); 
    //MÉTHODES PUBLIQUES 
    void Initialize(int initialHerbivores, int initialCarnivores, int initialPlants);
    void Update(float deltaTime); 
    void SpawnFood(int count); 
    void RemoveDeadEntities(); 
    void HandleReproduction(); 
    void HandleEating(); 
    //GETTERS 
    int GetEntityCount() const { return mEntities.size(); } 
    int GetFoodCount() const { return mFoodSources.size(); }
    std::vector<Food> Getfood() const { return mFoodSources; }
    Statistics GetStatistics() const { return mStats; } 
    float GetWorldWidth() const { return mWorldWidth; } 
    float GetWorldHeight() const { return mWorldHeight; }
    int GetMaxEntity() const { return mMaxEntities; }
    // MÉTHODES DE GESTION 
    void AddEntity(std::unique_ptr<Entity> entity); 
    void AddFood(Vector2D position, float energy = 25.0f); 
    //RENDU 
    void Render(SDL_Renderer* renderer) const; 
private: 
    //MÉTHODES PRIVÉES 
    void UpdateStatistics(); 
    void SpawnRandomEntity(EntityType type); 
    Vector2D GetRandomPosition() const; 
    void HandlePlantGrowth(float deltaTime); 
}; 
} // namespace Core 
} // namespace Ecosystem
```
Ce fichier contient lùensemble des méthodes appliquées à l'environnement pour lui donner un qspect dynamique:

### **Entity.h**

```cpp
#pragma once 
#include "Structs.h" 
#include <SDL3/SDL.h> 
#include <memory>
#include <random> 
#include <vector> 
namespace Ecosystem { 
namespace Core { 
// ÉNUMÉRATION DES TYPES D'ENTITÉS 
enum class EntityType { 
    HERBIVORE, 
    CARNIVORE, 
    PLANT 
}; 
class Entity { 
private: 
    // DONNÉES PRIVÉES - État interne protégé 
    float mEnergy; 
    float mMaxEnergy; 
    int mAge; 
    int mMaxAge; 
    bool mIsAlive; 
    Vector2D mVelocity; 
    EntityType mType; 
    
    // Générateur aléatoire 
    mutable std::mt19937 mRandomGenerator; 
public: 
    // DONNÉES PUBLIQUES - Accès direct sécurisé 
    Vector2D position; 
    Color color; 
    float size; 
    std::string name; 
    
    // CONSTRUCTEURS 
    Entity(EntityType type, Vector2D pos, std::string entityName = "Unnamed"); 
    Entity(const Entity& other);  // Constructeur de copie 
    
    // DESTRUCTEUR 
    ~Entity(); 
    
    //MÉTHODES PUBLIQUES 
    void Update(float deltaTime); 
    void Move(float deltaTime); 
    void Eat(float energy); 
    bool CanReproduce() const; 
    std::unique_ptr<Entity> Reproduce(); 
    void ApplyForce(Vector2D force); 
    
    // GETTERS - Accès contrôlé aux données privées 
    float GetEnergy() const { return mEnergy; } 
    float GetEnergyPercentage() const { return mEnergy / mMaxEnergy; } 
    int GetAge() const { return mAge; } 
    bool IsAlive() const { return mIsAlive; } 
    EntityType GetType() const { return mType; } 
    Vector2D GetVelocity() const { return mVelocity; } 
    
    // MÉTHODES DE COMPORTEMENT 
    Vector2D SeekFood(const std::vector<Food>& foodSources);
    Vector2D SeekFood( const std::vector<std::unique_ptr<Entity>>& EntityFood);
    Vector2D AvoidPredators(const std::vector<std::unique_ptr<Entity>>& predators) const; 
    Vector2D StayInBounds(float worldWidth, float worldHeight); 
    
    // MÉTHODE DE RENDU 
    void Render(SDL_Renderer* renderer) const; 
private: 
    // MÉTHODES PRIVÉES - Logique interne 
    void ConsumeEnergy(float deltaTime); 
    void Age(float deltaTime); 
    void CheckVitality(); 
    Vector2D GenerateRandomDirection(); 
    Color CalculateColorBasedOnState() const; 
}; 
} // namespace Core 
} // namespace Ecosystem
```
Ce fichier se propose de contenir le protoype des methodes et la declaration de la classe **Entity** primordiale dans le gestion dynamique de l'ecosysteme.

### **GameEngine**
```cpp
#include "../Graphics/Window.h" 
#include "Ecosystem.h" 
#include <chrono> 
namespace Ecosystem { 
namespace Core { 
class GameEngine { 
private: 
    // ÉTAT DU MOTEUR 
    Graphics::Window mWindow; 
    Ecosystem mEcosystem; 
    bool mIsRunning; 
    bool mIsPaused; 
    float mTimeScale; 
     
    // ⏱ CHRONOMÉTRE 
    std::chrono::high_resolution_clock::time_point mLastUpdateTime; 
    float mAccumulatedTime; 
public: 
    // 🏗 CONSTRUCTEUR 
    GameEngine(const std::string& title, float width, float height); 
     
    // MÉTHODES PRINCIPALES 
    bool Initialize(); 
    void Run(); 
    void Shutdown(); 
     
    // GESTION D'ÉVÉNEMENTS 
    void HandleEvents(); 
    void HandleInput(SDL_Keycode key); 
private: 
    // MÉTHODES INTERNES 
    void Update(float deltaTime); 
    void Render(); 
    void RenderUI(); 
}; 
} // namespace Core 
} // namespace Ecosystem
```

### **Structs.h**
```cpp
#pragma once
#include <cstdint> 
#include <string> 
#include <cmath> 
#include <iostream>
namespace Ecosystem { 
namespace Core { 
//STRUCTS POUR LES DONNÉES SIMPLES 
struct Vector2D { 
    float x; 
    float y; 
    // 

    //Constructeur avec valeurs par défaut 
    Vector2D(float xValue = 0.0f, float yValue = 0.0f) : x(xValue), y(yValue) {
        std::cout << "✅vector cree : (" << x <<"," << y <<")";
    } 
    
    // Méthodes utilitaires 
    float Distance(const Vector2D& other) const { 
        float dx = x - other.x; 
        float dy = y - other.y; 
        return std::sqrt(dx * dx + dy * dy); 
    }
    Vector2D operator+(const Vector2D& other) const { 
        return Vector2D(x + other.x, y + other.y); 
    }
    Vector2D operator*(float scalar) const { 
        return Vector2D(x * scalar, y * scalar); 
    }
 }; 
struct Color { 
    uint8_t r; 
    uint8_t g; 
    uint8_t b; 
    uint8_t a; 
    
    // Constructeurs multiples 
    Color() : r(255), g(255), b(255), a(255) {}  // Blanc par défaut 
    Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255)  
        : r(red), g(green), b(blue), a(alpha) {} 
    
        // Couleurs prédéfinies 
    static Color Red() { return Color(255, 0, 0); } 
    static Color Green() { return Color(0, 255, 0); } 
    static Color Blue() { return Color(0, 0, 255); }
    static Color Yellow() { return Color(255, 255, 0); }  
}; 
    
struct Food { 
    Vector2D position; 
    float energyValue; 
    Color color; 
    
    // Constructeur 
    Food(Vector2D pos, float energy = 25.0f)  
        : position(pos), energyValue(energy), color(Color::Green()) {} 
}; 
} // namespace Core 
} // namespace Ecosystem
```
ce fichier contien l'ensemble des structures de bases de manipulation du plan et des type d'elements de l'ecosysteme.

## **Fichiers source**
### **src/Core/Ecosystem.cpp**
```cpp
#include "Core/Ecosystem.h" 
#include <algorithm> 
#include <iostream> 
namespace Ecosystem { 
namespace Core { 
// 🏗 CONSTRUCTEUR 
Ecosystem::Ecosystem(float width, float height, int maxEntities) 
    : mWorldWidth(width), mWorldHeight(height), mMaxEntities(maxEntities), 
      mDayCycle(0), mRandomGenerator(std::random_device{}()) 
{ 
    // Initialisation des statistiques 
    mStats = {0, 0, 0, 0, 0, 0}; 
    std::cout << "🌍Écosystème créé: " << width << "x" << height << std::endl; 
} 
// 🗑 DESTRUCTEUR 
Ecosystem::~Ecosystem() { 
    std::cout << "🌍Écosystème détruit (" << mEntities.size() << " entités nettoyé" << std::endl;
 } 
// INITIALISATION 
void Ecosystem::Initialize(int initialHerbivores, int initialCarnivores, int initialPlants) {
    mEntities.clear(); 
    mFoodSources.clear(); 
    // Création des entités initiales 
    for (int i = 0; i < initialHerbivores; ++i) { 
        SpawnRandomEntity(EntityType::HERBIVORE); 
    }
    for (int i = 0; i < initialCarnivores; ++i) { 
        SpawnRandomEntity(EntityType::CARNIVORE); 
    }
    for (int i = 0; i < initialPlants; ++i) { 
        SpawnRandomEntity(EntityType::PLANT); 
    }
    // Nourriture initiale 
    SpawnFood(20); 
    std::cout << "🌱Écosystème initialisé avec " << mEntities.size() << " entités" <<std::endl;
} 
// MISE À JOUR 
void Ecosystem::Update(float deltaTime) { 
    // Mise à jour de toutes les entités 
    for (auto& entity : mEntities) { 
        entity->Update(deltaTime); 
    }
    // Gestion des comportements 
    HandleEating(); 
    HandleReproduction(); 
    RemoveDeadEntities(); 
    HandlePlantGrowth(deltaTime); 
    // Mise à jour des statistiques 
    UpdateStatistics();
    mDayCycle++; 
} 
// GÉNÉRATION DE NOURRITURE 
void Ecosystem::SpawnFood(int count) { 
    for (int i = 0; i < count; ++i) { 
        if (mFoodSources.size() < 100) {  // Limite maximale de nourriture 
            Vector2D position = GetRandomPosition(); 
            mFoodSources.emplace_back(position, 25.0f); 
        } 
    }
 } 
// SUPPRESSION DES ENTITÉS MORTES 
void Ecosystem::RemoveDeadEntities() { 
    int initialCount = mEntities.size(); 
    mEntities.erase( 
        std::remove_if(mEntities.begin(), mEntities.end(), 
            [](const std::unique_ptr<Entity>& entity) {  
                return !entity->IsAlive();  
            }), 
        mEntities.end() 
    ); 
    int removedCount = initialCount - mEntities.size(); 
    if (removedCount > 0) { 
        mStats.deathsToday += removedCount; 
    }
 } 
// GESTION DE LA REPRODUCTION 
void Ecosystem::HandleReproduction() { 
    std::vector<std::unique_ptr<Entity>> newEntities; 
    for (auto& entity : mEntities) { 
        if (entity->CanReproduce() && mEntities.size() < mMaxEntities) { 
            auto baby = entity->Reproduce(); 
            if (baby) { 
                newEntities.push_back(std::move(baby)); 
                mStats.birthsToday++; 
            }
        } 
    } 
                
    // Ajout des nouveaux entités 
    for (auto& newEntity : newEntities) { 
        mEntities.push_back(std::move(newEntity)); 
    }
 } 
// 🍽 GESTION DE L'ALIMENTATION 
void Ecosystem::HandleEating() {
    // Ici on implémenterait la logique de recherche de nourriture
    for (auto& entity : mEntities) {
        if (entity->GetType() == EntityType::HERBIVORE) {
            if (entity->GetEnergy() < 40.0f) {
                entity->position.x = entity->position.x + (entity->SeekFood(mFoodSources).x * 0.05f);
                entity->position.y = entity->position.y + (entity->SeekFood(mFoodSources).y * 0.05f);
                entity->StayInBounds(1200.0f, 600.0f);
                if (sqrt(entity->SeekFood(mFoodSources).x * entity->SeekFood(mFoodSources).x + entity->SeekFood(mFoodSources).y * entity->SeekFood(mFoodSources).y) < 15.0f) {
                    entity->Eat(50.0f);  
                }
            }
            entity->position.x = entity->position.x + entity->AvoidPredators(mEntities).x * 2.5f;
            entity->position.y = entity->position.y + entity->AvoidPredators(mEntities).y * 2.5f;
            entity->StayInBounds(1200.0f, 600.0f);
        }
        if (entity->GetType() == EntityType::CARNIVORE && entity->GetEnergy() < 80.0f) {
            entity->position.x += (entity->SeekFood(mEntities).x * 2.75f);
            entity->position.y += (entity->SeekFood(mEntities).y * 2.75f);
            if (std::sqrt(entity->position.x * entity->position.x + entity->position.y * entity->position.y) <= 18.0f) {
                entity->Eat(100.0f);
            }
        }
    }
    // Pour l'instant, gestion simplifiée 
    for (auto& entity : mEntities) { 
        if (entity->GetType() == EntityType::PLANT) { 
            // Les plantes génèrent de l'énergie 
            entity->Eat(0.1f); 
        } 
    }
 } 
// MISE À JOUR DES STATISTIQUES 
void Ecosystem::UpdateStatistics() { 
    mStats.totalHerbivores = 0; 
    mStats.totalCarnivores = 0; 
    mStats.totalPlants = 0; 
    mStats.totalFood = mFoodSources.size(); 
    for (const auto& entity : mEntities) { 
        switch (entity->GetType()) { 
            case EntityType::HERBIVORE: 
                mStats.totalHerbivores++; 
                break; 
            case EntityType::CARNIVORE: 
                mStats.totalCarnivores++; 
                break; 
            case EntityType::PLANT: 
                mStats.totalPlants++; 
                break; 
        } 
    }
 } 
// CRÉATION D'ENTITÉ ALÉATOIRE 
void Ecosystem::SpawnRandomEntity(EntityType type) { 
    if (mEntities.size() >= mMaxEntities) return; 
    Vector2D position = GetRandomPosition();
    std::string name; 
    switch (type) { 
        case EntityType::HERBIVORE: 
            name = "Herbivore_" + std::to_string(mStats.totalHerbivores); 
            break; 
        case EntityType::CARNIVORE: 
            name = "Carnivore_" + std::to_string(mStats.totalCarnivores); 
            break; 
        case EntityType::PLANT: 
            name = "Plant_" + std::to_string(mStats.totalPlants); 
            break; 
    }
    mEntities.push_back(std::make_unique<Entity>(type, position, name));
} 
// POSITION ALÉATOIRE 
Vector2D Ecosystem::GetRandomPosition() const { 
    std::uniform_real_distribution<float> distX(0.0f, mWorldWidth); 
    std::uniform_real_distribution<float> distY(0.0f, mWorldHeight); 
    return Vector2D(distX(mRandomGenerator), distY(mRandomGenerator)); 
} 
// CROISSANCE DES PLANTES 
void Ecosystem::HandlePlantGrowth(float deltaTime) { 
    // Occasionnellement, faire pousser de nouvelles plantes 
    std::uniform_real_distribution<float> chance(0.0f, 1.0f); 
    if (chance(mRandomGenerator) < 0.01f && mEntities.size() < mMaxEntities) { 
        SpawnRandomEntity(EntityType::PLANT); 
    }
 } 
//RENDU 
void Ecosystem::Render(SDL_Renderer* renderer) const { 
    // Rendu de la nourriture 
    for (const auto& food : mFoodSources) { 
        SDL_FRect rect = { 
            food.position.x - 3.0f, 
            food.position.y - 3.0f, 
            6.0f, 
            6.0f 
        };
        SDL_SetRenderDrawColor(renderer, food.color.r, food.color.g, food.color.b, food.color.a); 
        SDL_RenderFillRect(renderer, &rect); 
    }
    // Rendu des entités 
    for (const auto& entity : mEntities) { 
        entity->Render(renderer); 
    }
} 
void Ecosystem::AddEntity(std::unique_ptr<Entity> entity) {
   if (mEntities.size() >= mMaxEntities) {
        std::cout<< "❌ nombre maximal atteint !!"<<std::endl;
    }
    else {
        //ajout de la nouvelle entite
        mEntities.push_back(std::move(entity));
        //mise a jour des stats
        UpdateStatistics();
        std::cout<< "🍖 nouvelle entite ajoutee!!" <<std::endl;
    }
}
} // namespace Core 
} // namespace Ecosystem
```
operations effectuées sur l'écosysteme/

### **src/Core/Entity.cpp**
```cpp
#include "Core/Entity.h"
#include <cmath>
#include <iostream>
#include <algorithm>
namespace Ecosystem {
namespace Core {
// 🏗 CONSTRUCTEUR PRINCIPAL
Entity::Entity(EntityType type, Vector2D pos, std::string entityName)
    : mType(type), position(pos), name(entityName),
      mRandomGenerator(std::random_device{}())  // Initialisation du générateur alé
 {
    //INITIALISATION SELON LE TYPE
    switch(mType) {
        case EntityType::HERBIVORE:
            mEnergy = 80.0f;
            mMaxEnergy = 150.0f;
            mMaxAge = 200;
            color = Color::Blue();
            size = 8.0f;
            break;
        case EntityType::CARNIVORE:
            mEnergy = 100.0f;
            mMaxEnergy = 200.0f;
            mMaxAge = 150;
            color = Color::Red();
            size = 12.0f;
            break;
        case EntityType::PLANT:
            mEnergy = 50.0f;
            mMaxEnergy = 100.0f;
            mMaxAge = 300;
            color = Color::Green();
            size = 6.0f;
            break;
    }
    mAge = 0;
    mIsAlive = true;
    mVelocity = GenerateRandomDirection();
    std::cout << "🌱 Entité créée: " << name << " à (" << position.x << ", " << position.y << ")";
 }

// 🏗 CONSTRUCTEUR DE COPIE
Entity::Entity(const Entity& other)
    : mType(other.mType), position(other.position), name(other.name + "_copy"),
      mEnergy(other.mEnergy * 0.7f),  // Enfant a moins d'énergie
      mMaxEnergy(other.mMaxEnergy),
      mAge(0),  // Nouvelle entité, âge remis à 0
      mMaxAge(other.mMaxAge),
      mIsAlive(true),
      mVelocity(other.mVelocity),
      color(other.color),
      size(other.size * 0.8f),  // Enfant plus petit
      mRandomGenerator(std::random_device{}())
{

    std::cout << " 👶 Copie d'entité créée: " << name << std::endl;
}
// 🗑 DESTRUCTEUR
Entity::~Entity() {
    std::cout << "💀Entité détruite: " << name << " (Âge: " << mAge << ")" << std::endl;
 }
 //MISE À JOUR PRINCIPALE
void Entity::Update(float deltaTime) {
    if (!mIsAlive) return;
    //PROCESSUS DE VIE
    ConsumeEnergy(deltaTime);
    Age(deltaTime);
    Move(deltaTime);
    CheckVitality();
}
//MOUVEMENT
void Entity::Move(float deltaTime) {
    if (mType == EntityType::PLANT) return;  // Les plantes ne bougent pas
    //Comportement aléatoire occasionnel
    std::uniform_real_distribution<float> chance(0.0f, 1.0f);
    if (chance(mRandomGenerator) < 0.02f) {
        mVelocity = GenerateRandomDirection();
    }
    //intervalle du mouvement
    position = StayInBounds(1200.0f, 800.0f); // definition du cadre dans lequel doit se deplqcer les enties 
    //Application du mouvement
    position = position + mVelocity * deltaTime * 20.0f;    
    // Consommation d'énergie due au mouvement
    mEnergy -= mVelocity.Distance(Vector2D(0, 0)) * deltaTime * 0.1f;
}
// 🍽 MANGER
 void Entity::Eat(float energy) {
    mEnergy += energy;
    if (mEnergy > mMaxEnergy) {
        mEnergy = mMaxEnergy;
    }
        std::cout << "🍽 " << name << " mange et gagne " << energy << " énergie" << std::endl;
    }

    //CONSOMMATION D'ÉNERGIE
void Entity::ConsumeEnergy(float deltaTime) {
    float baseConsumption = 0.0f;
    switch(mType) {
        case EntityType::HERBIVORE:
            baseConsumption = 1.5f;
            break;
        case EntityType::CARNIVORE:
            baseConsumption = 2.0f;
            break;
        case EntityType::PLANT:
            baseConsumption = -0.5f;  // Les plantes génèrent de l'énergie !
            break;
    }
    mEnergy -= baseConsumption * deltaTime;
}

// VIEILLISSEMENT
void Entity::Age(float deltaTime) {
    mAge += static_cast<int>(deltaTime * 10.0f);  // Accéléré pour la simulation
}

// ❤VÉRIFICATION DE LA SANTÉ
void Entity::CheckVitality() {
    if (mEnergy <= 0.0f || mAge >= mMaxAge) {
        mIsAlive = false;
        std::cout << "💀 " << name << " meurt - ";
        if (mEnergy <= 0) std::cout << "Faim";
        else std::cout << "Vieillesse";
        std::cout << std::endl;
    }
 }

 // REPRODUCTION
bool Entity::CanReproduce() const {
    return mIsAlive && mEnergy > mMaxEnergy * 0.8f && mAge > 20;
}
std::unique_ptr<Entity> Entity::Reproduce() {
    if (!CanReproduce()) return nullptr;
    // Chance de reproduction
    std::uniform_real_distribution<float> chance(0.0f, 1.0f);
    if (chance(mRandomGenerator) < 0.3f) {
        mEnergy *= 0.6f;  // Coût énergétique de la reproduction
        return std::make_unique<Entity>(*this);  // Utilise le constructeur de copi
    }
    return nullptr;
}

// GÉNÉRATION DE DIRECTION ALÉATOIRE
Vector2D Entity::GenerateRandomDirection() {
    std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
    return Vector2D(dist(mRandomGenerator), dist(mRandomGenerator));
}

// CALCUL DE LA COULEUR BASÉE SUR L'ÉTAT
Color Entity::CalculateColorBasedOnState() const {
    float energyRatio = GetEnergyPercentage();
    Color baseColor = color;
    // Rouge si faible énergie
    if (energyRatio < 0.3f) {
        baseColor.r = 255;
        baseColor.g = static_cast<uint8_t>(baseColor.g * energyRatio);
        baseColor.b = static_cast<uint8_t>(baseColor.b * energyRatio);
    }
    return baseColor;
}
// RENDU GRAPHIQUE
void Entity::Render(SDL_Renderer* renderer) const {
    if (!mIsAlive) return;
    Color renderColor = CalculateColorBasedOnState();
    SDL_FRect rect = {
        position.x - size / 2.0f,
        position.y - size / 2.0f,
        size,
        size
    };
    SDL_SetRenderDrawColor(renderer, renderColor.r, renderColor.g, renderColor.b, renderColor.a);
    SDL_RenderFillRect(renderer, &rect);

    // Indicateur d'énergie (barre de vie)
    if (mType != EntityType::PLANT) {
        float energyBarWidth = size * GetEnergyPercentage();
        SDL_FRect energyBar = {
            position.x - size / 2.0f,
            position.y - size / 2.0f - 3.0f,
            energyBarWidth,
            2.0f
        };
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &energyBar);
    }
 }
Vector2D Entity::StayInBounds(float worldWidth, float worldHeight) {
    if (position.x > worldWidth - 6.0f) position.x = worldWidth - 6.0f;
    if (position.y > worldHeight - 6.0f) position.y = worldHeight - 6.0f;
    if (position.x < 6.0f) position.x = 6.0f;
    if (position.y < 6.0f) position.y = 6.0f;
    return position;
}
Vector2D Entity::SeekFood(const std::vector<Food>& foodRsources) {
    int count;
    Vector2D posTemp = position;
    Vector2D foodPos;
    float distMin = 2000.0f;
    float distance;
    for (auto& food : foodRsources) {
        distance = posTemp.Distance(food.position);
        if (distMin > distance && distance < 200.0f) {
            distMin = distance;
            foodPos = food.position;
        }
    }
    posTemp.x = foodPos.x - posTemp.x;
    posTemp.y = foodPos.y - posTemp.y;
    return posTemp;
}

Vector2D Entity::SeekFood(const std::vector<std::unique_ptr<Entity>>& EntityFood) {
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
                foodPos = entity->position;
            }
        }
    }
    predatorPos.x = (foodPos.x - predatorPos.x) / distMin;
    predatorPos.y = (foodPos.y - predatorPos.y) / distMin;
    return predatorPos;
}

Vector2D Entity::AvoidPredators(const std::vector<std::unique_ptr<Entity>>& predators) const {
    Vector2D herbipos = position ;
    Vector2D predatorPos;
    float distMin = 3000.0f;
    float dist;
    for (auto& entity : predators) {
        if (entity->GetType() == EntityType::CARNIVORE) {
            dist = herbipos.Distance(entity->position);
            if (distMin > dist && dist <= 15.0f) {
                distMin = dist;
                predatorPos = entity->position;
            }
        }
    }
    herbipos.x = ((herbipos.x - predatorPos.x) + 100.0f) / distMin;
    herbipos.y = ((herbipos.y - predatorPos.y) +100.0f) / distMin; 
    return herbipos;
}
void Entity::ApplyForce(Vector2D force) {
    mVelocity = mVelocity + force;
}
} // namespace Core
} // namespace Ecosystem
```
### **main.cpp**
```cpp
#include "Core/GameEngine.h" 
#include <iostream> 
#include <cstdlib> 
#include <ctime> 
#include <windows.h>

int main(int argc, char* argv[]) {
    SetConsoleOutputCP(CP_UTF8); 
    // Initialisation de l'aléatoire 
    std::srand(static_cast<unsigned int>(std::time(nullptr))); 
     
    std::cout << "🎮Démarrage du Simulateur d'Écosystème" << std::endl; 
    std::cout << "=======================================" << std::endl; 
     
    // 🏗 Création du moteur de jeu 
    Ecosystem::Core::GameEngine engine("Simulateur d'Écosystème Intelligent", 1200.0f, 800.0f);
     
    // Initialisation 
    if (!engine.Initialize()) { 
        std::cerr << "❌Erreur: Impossible d'initialiser le moteur de jeu" << std::endl;
        return -1; 
    }
     
    std::cout << "✅Moteur initialisé avec succès" << std::endl; 
    std::cout << "🎯Lancement de la simulation..." << std::endl; 
    std::cout << "=== CONTRÔLES ===" << std::endl; 
    std::cout << "ESPACE: Pause/Reprise" << std::endl; 
    std::cout << "R: Reset simulation" << std::endl; 
    std::cout << "F: Ajouter nourriture" << std::endl; 
    std::cout << "FLÈCHES: Vitesse simulation" << std::endl; 
    std::cout << "ÉCHAP: Quitter" << std::endl; 
     
    // Boucle principale 
    engine.Run(); 
     
    // Arrêt propre 
    engine.Shutdown(); 
     
    std::cout << "👋Simulation terminée. Au revoir !" << std::endl; 
    return 0;
}
```
# **code de compilation**
```
 g++ -std=c++17 -Iinclude -o ecosystem src/*.cpp src/Core/*.cpp src/Graphics/*.cpp -o ecosysteme.exe -lSDl3
 ```
 ## **resultat attendu**
 Une fenetre dans laquelle se d"plqce des cqrnivore en rouge et herbivore en bleu. Les cqrnivore chqssqnt les herbivores lorsaue leur niveau d'energie est bas et des herbivores cherchant de la nourriture quand le leur l'est aussi. les herbivorres se reproduisent selon un taux de chance plus élevé aue celui des carnivores et fuient ceux ci tant qu'ils n'ont pas faim et quand ils se trouvent dans un certain perimetre.