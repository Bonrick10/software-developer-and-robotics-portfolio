import React from 'react';

export const AuthContext = React.createContext();

export function AuthProvider ({ children }) {
  const [token, setToken] = React.useState('null');

  const config = {
    headers: {
      Authorization: `Bearer ${token}`
    }
  }

  const setLocalToken = (token) => {
    setToken(token);
    localStorage.setItem('token', token);
  };

  return (
    <AuthContext.Provider value={{ token, config, setToken, setLocalToken }}>
      {children}
    </AuthContext.Provider>
  )
}
