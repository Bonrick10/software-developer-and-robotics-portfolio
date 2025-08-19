import React from 'react';
import { BrowserRouter, Routes, Route, Navigate } from 'react-router-dom';
import { Login, Register, Dashboard, Edit, Preview, Rearrange } from './pages'
import { AuthProvider, StoreProvider } from './contexts';

import 'bootstrap/dist/css/bootstrap.min.css';
import './App.css';

function App () {
  return (
    <AuthProvider>
      <BrowserRouter>
        <Routes>
          <Route path="/register" element={<Register/>} />
          <Route path="/login" element={<Login/>} />
          <Route path="/dashboard" element={<Dashboard/>} />
          <Route path="/editPres/:presId/slide/:slideIndex" element={<Edit />}/>
          <Route path="/preview" element={<Preview/>} />
          <Route path="/rearrange/:presId" element={
            <StoreProvider>
              <Rearrange/>
            </StoreProvider>
          } />
          <Route path="*" element={<Navigate to="/login" replace />}/>
        </Routes>
      </BrowserRouter>
    </AuthProvider>
  );
}

export default App;
