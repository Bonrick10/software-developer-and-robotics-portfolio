import React from 'react';
import axios from 'axios';
import { useNavigate, Navigate } from 'react-router-dom';
import { Button, Form } from 'react-bootstrap';

import { AuthContext } from '../contexts';
import { AuthNavBar } from '../components/auth';

export function Register () {
  const { token, setLocalToken } = React.useContext(AuthContext);
  console.log(token);
  if (token !== 'null') {
    return <Navigate to ="/dashboard" />
  }

  const [email, setEmail] = React.useState('');
  const [name, setName] = React.useState('');
  const [password, setPassword] = React.useState('');
  const [confirmPassword, setConfirmPassword] = React.useState('');
  const navigate = useNavigate();

  const register = async () => {
    if (email.length < 1) {
      alert('Email is too short');
    } else if (name.length < 1) {
      alert('Name is too short');
    } else if (password.length < 1) {
      alert('Password is too short');
    } else if (password !== confirmPassword) {
      alert('passwords do not match');
    } else {
      try {
        const response = await axios.post('http://localhost:5005/admin/auth/register', {
          email,
          password,
          name
        })
        setLocalToken(response.data.token);
        navigate('/dashboard')
        console.log(response.data.token);
      } catch (err) {
        alert(err.response.data.error);
      }
    }
  }

  const enterKeyDown = (event) => {
    if (event.key === 'Enter') {
      register();
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
          <Form.Label>Name</Form.Label>
          <Form.Control
            type="text"
            placeholder="John Doe"
            onChange={e => setName(e.target.value)}
            onKeyDown={enterKeyDown}
            value={name}
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
        <Form.Group className="mb-3">
          <Form.Label>Confirm Password</Form.Label>
          <Form.Control
            type="password"
            onChange={e => setConfirmPassword(e.target.value)}
            onKeyDown={enterKeyDown}
            value={confirmPassword}
          />
        </Form.Group>
        <Button variant="primary" onClick={register}>Register</Button>
      </Form>
  </>
  );
}
