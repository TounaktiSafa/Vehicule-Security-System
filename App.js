import React, { useState } from 'react';
import { NavigationContainer } from '@react-navigation/native';
import { createStackNavigator } from '@react-navigation/stack';
import SignIn from './signin';
import SignUp from './signup';
import Home from './Home';

const Stack = createStackNavigator();

export default function App() {
  const [users, setUsers] = useState([]); // Liste des utilisateurs enregistrés
  const [currentUser, setCurrentUser] = useState(null); // Utilisateur connecté

  return (
    <NavigationContainer>
      <Stack.Navigator>
        {!currentUser ? (
          <>
            <Stack.Screen
              name="SignIn"
              options={{ title: 'Connexion' }}
            >
              {(props) => <SignIn {...props} users={users} onLogin={setCurrentUser} />}
            </Stack.Screen>
            <Stack.Screen
              name="SignUp"
              options={{ title: 'Inscription' }}
            >
              {(props) => <SignUp {...props} onRegister={(user) => setUsers([...users, user])} />}
            </Stack.Screen>
          </>
        ) : (
          <Stack.Screen
            name="Home"
            options={{ title: 'Accueil' }}
          >
            {(props) => <Home {...props} user={currentUser} onLogout={() => setCurrentUser(null)} />}
          </Stack.Screen>
        )}
      </Stack.Navigator>
    </NavigationContainer>
  );
}
