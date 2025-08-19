import React from 'react';
import { useNavigate, Navigate } from 'react-router-dom';
import axios from 'axios';
import { Button, Form } from 'react-bootstrap';

import { AuthContext } from '../contexts';
import { AuthNavBar } from '../components/auth';

export function Login () {
  const { token, setToken, setLocalToken } = React.useContext(AuthContext);
  const lsToken = localStorage.getItem('token');
  if (lsToken !== 'null') {
    setToken(lsToken);
  }

  if (token !== 'null') {
    return <Navigate to ="/dashboard" />
  }

  const [email, setEmail] = React.useState('');
  const [password, setPassword] = React.useState('');
  const navigate = useNavigate();

  const login = async () => {
    if (email.length < 1) {
      alert('Email not given');
    } else if (password.length < 1) {
      alert('Password not given');
    } else {
      try {
        const response = await axios.post('http://localhost:5005/admin/auth/login', {
          email,
          password,
        })
        setLocalToken(response.data.token);
        navigate('/dashboard');
      } catch (err) {
        alert(err.response.data.error);
      }
    }
  }

  const enterKeyDown = (event) => {
    if (event.key === 'Enter') {
      login();
    }
  }

  return (
    <>
      <AuthNavBar></AuthNavBar>
      <Form>
        <Form.Group className="mb-3">
          <Form.Label>Email address</Form.Label>
          <Form.Control
            type="email"
            placeholder="name@example.com"
            onChange={e => setEmail(e.target.value)}
            onKeyDown={enterKeyDown}
            value={email}
          />
        </Form.Group>
        <Form.Group className="mb-3">
          <Form.Label>Password</Form.Label>
          <Form.Control
            type="password"
            onChange={e => setPassword(e.target.value)}
            onKeyDown={enterKeyDown}
            value={password}
          />
        </Form.Group>
        <Button variant="primary" onClick={login}>Login</Button>
      </Form>
    </>
  );
}
